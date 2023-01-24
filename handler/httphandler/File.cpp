#include "../AcceptedSocket.hpp"
#include "File.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

File::FileError::FileError()
:std::runtime_error(strerror(errno))
{
}

File::FileError::FileError(const std::string &func_name)
:std::runtime_error(func_name + std::string(": ") + strerror(errno))
{
}

File::FileError::~FileError() throw()
{
}

File::File()
{
}

File::File(
	ISubject * subject,
	std::list<ICommand *> * commands,
	HTTPMethod *method,
	const std::string & path,
	AcceptedSocket *as,
	bool autoindex,
	const std::string & index_file
)
:HTTPMethodReceiver(subject, commands, method, as, path),
_nb(0),
_autoindex(autoindex),
_index_file(index_file)
{
}

File::~File()
{
	::close(_fd);
}

void File::update(int event)
{
	if (event & (POLLHUP | POLLERR | POLLNVAL)) {
		getAcceptedSocket()->createResponse("error");
		getSubject()->unsubscribe(_fd, false);
		return ;
	}
	if (event & POLLOUT) {
		getCommandList()->push_back(getWriteCommand());
	} else if (event == POLLIN) {
		getCommandList()->push_back(getReadCommand());
	}
}

int File::read()
{
	char buff[BUFSIZE];
	ssize_t nb = ::read(_fd, buff, BUFSIZE);
	if (nb < 0) {
		getSubject()->unsubscribe(_fd, false);
		return -1;
	} else if (nb == 0) {
		getAcceptedSocket()->createResponse(_buff);
		getSubject()->unsubscribe(_fd, false);
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	return 1;
}

int File::write()
{
	ssize_t nb = ::write(_fd, _buff.c_str(), _buff.size());
	if (nb == -1) {
		getSubject()->unsubscribe(_fd, false);
		return -1;
	}
	if (nb < _nb + _buff.size()) {
		_buff = _buff.substr(0, nb);
		_nb += nb;
		return 1;
	}
	std::cout << "write buffer" << _buff << std::endl;
	getSubject()->unsubscribe(_fd, false);
	getAcceptedSocket()->createResponse("created");
	return 0;
}

int File::httpGet()
{
	if (execStat() == -1) {
		if (errno ==  ENOENT) {
			getAcceptedSocket()->setStatus(NOT_FOUND);
		} else if(errno == EACCES) {
			getAcceptedSocket()->setStatus(FORBIDDEN);
		} else {
			getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
		}
		return -1;
	}
	if (checkPermission(S_IROTH) == false) {
		getAcceptedSocket()->setStatus(FORBIDDEN);
		return -1;
	}
	if (isDirectory() == true) {
		if (_path[_path.size() - 1] != '/') {
			_path.push_back('/');
		}
		if (_autoindex == true) {
			return processAutoindex();
		} else if (_index_file != "default") {
			_path += _index_file;
		} else {
			getAcceptedSocket()->setStatus(NOT_FOUND);
			return -1;
		}
	}
	_fd = open(getPath().c_str(), O_RDONLY | O_CLOEXEC);
	if (_fd == -1) {
		getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
		return -1;
	}
	getSubject()->subscribe(_fd, POLLIN, this);
	return 0;
}

int File::httpPost()
{
	if (execStat() == -1) {
		if (errno ==  ENOENT) {
			_fd = open(getPath().c_str(), O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC);
		} else if(errno == EACCES) {
			getAcceptedSocket()->setStatus(FORBIDDEN);
			return -1;
		} else {
			getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
			return -1;
		}
	} else {
		_fd = open(getPath().c_str(), O_WRONLY | O_APPEND | O_CLOEXEC);
	}
	if (_fd == -1) {
		getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
		return -1;
	}
	if (isRegularFile() == false) {
		getAcceptedSocket()->setStatus(FORBIDDEN);
		return -1;
	}
	if (checkPermission(S_IWOTH) == false) {
		getAcceptedSocket()->setStatus(FORBIDDEN);
		return -1;
	}
	//_buff = "value=aaaa&value_2=bbbb";
	getSubject()->subscribe(_fd, POLLOUT, this);
	return 0;
}

int File::httpDelete()
{
	std::string target = _path;
	_path = _path.substr(_path.rfind('/'));
	if (execStat() == -1) {
		if (errno ==  ENOENT) {
			getAcceptedSocket()->setStatus(NOT_FOUND);
		} else if(errno == EACCES) {
			getAcceptedSocket()->setStatus(FORBIDDEN);
		} else {
			getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
		}
		return -1;
	}
	if (checkPermission(S_IXOTH) == false) {
		getAcceptedSocket()->setStatus(FORBIDDEN);
	}
	if (unlink(target.c_str()) == -1) {
		if (errno == EACCES) {
			getAcceptedSocket()->setStatus(UNAUTHORIZED);
		} else if (errno == EBUSY) {
			getAcceptedSocket()->setStatus(CONFLICT);
		} else {
			getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
		}
		return -1;
	}
	return 0;
}

int File::getFd() const {
	return _fd;
}
