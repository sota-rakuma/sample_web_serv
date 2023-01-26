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
	AcceptedSocket *as,
	bool autoindex,
	const std::string & index_file
)
:HTTPMethodReceiver(subject, commands, as),
_nb(0),
_autoindex(autoindex),
_index_file(index_file)
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
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		getSubject()->unsubscribe(_fd, false);
		return -1;
	} else if (nb == 0) {
		//getAcceptedSocket()->createResponse(_buff);
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
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		getSubject()->unsubscribe(_fd, false);
		return -1;
	}
	if (nb < _nb + _buff.size()) {
		_buff = _buff.substr(0, nb);
		_nb += nb;
		return 1;
	}
	getSubject()->unsubscribe(_fd, false);
	return 0;
}

int File::httpGet()
{
	if (execStat() == -1) {
		if (errno ==  ENOENT) {
			entrustCreateResponse(NOT_FOUND);
		} else if(errno == EACCES) {
			entrustCreateResponse(FORBIDDEN);
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
		}
		return -1;
	}
	if (checkPermission(S_IROTH) == false) {
		entrustCreateResponse(FORBIDDEN);
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
			entrustCreateResponse(NOT_FOUND);
			return -1;
		}
	}
	_fd = open(getPath().c_str(), O_RDONLY | O_CLOEXEC);
	if (_fd == -1) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return -1;
	}
	getSubject()->subscribe(_fd, POLLIN, this);
	entrustCreateResponse(OK);
	return 0;
}

int File::httpPost()
{
	if (execStat() == -1) {
		if (errno ==  ENOENT) {
			_fd = open(getPath().c_str(), O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC);
			setHTTPStatus(CREATED);
		} else if(errno == EACCES) {
			entrustCreateResponse(FORBIDDEN);
			return -1;
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
			return -1;
		}
	} else {
		_fd = open(getPath().c_str(), O_RDWR | O_APPEND | O_CLOEXEC);
		setHTTPStatus(OK);
	}
	if (_fd == -1) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return -1;
	}
	if (isRegularFile() == false) {
		entrustCreateResponse(FORBIDDEN);
		return -1;
	}
	if (checkPermission(S_IWOTH) == false) {
		entrustCreateResponse(FORBIDDEN);
		return -1;
	}
	getSubject()->subscribe(_fd, POLLOUT, this);
	// POSTした内容をbufferに格納
	httpGet();
	return 0;
}

int File::httpDelete()
{
	std::string target = _path;
	_path = _path.substr(_path.rfind('/'));
	if (execStat() == -1) {
		if (errno ==  ENOENT) {
			entrustCreateResponse(NOT_FOUND);
		} else if(errno == EACCES) {
			entrustCreateResponse(FORBIDDEN);
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
		}
		return -1;
	}
	if (checkPermission(S_IXOTH) == false) {
		entrustCreateResponse(FORBIDDEN);
		return -1;
	}
	if (unlink(target.c_str()) == -1) {
		if (errno == EACCES) {
			entrustCreateResponse(UNAUTHORIZED);
		} else if (errno == EBUSY) {
			entrustCreateResponse(CONFLICT);
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
		}
		return -1;
	}
	entrustCreateResponse(OK);
	return 0;
}

int File::getFd() const {
	return _fd;
}
