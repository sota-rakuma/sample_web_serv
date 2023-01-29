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
		getSubject()->unsubscribe(_fd, true);
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
	ssize_t nb = ::read(_fd, buff, BUFSIZE - 1);
	if (nb < 0) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		getSubject()->unsubscribe(_fd, true);
		return -1;
	} else if (nb == 0) {
		getAcceptedSocket()->createResponse();
		getSubject()->unsubscribe(_fd, true);
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
		getSubject()->unsubscribe(_fd, true);
		return -1;
	}
	if (nb < _buff.size()) {
		_buff = _buff.substr(nb + 1);
		return 1;
	}
	//_buff = "";
	getSubject()->unsubscribe(_fd, true);
	getAcceptedSocket()->createResponse();
	return 0;
}

int File::httpGet()
{
	if (execStat() == -1) {
		if (errno == ENOENT) {
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
			//return processAutoindex();
		} else if (_index_file.size() > 0) {
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
	setHTTPStatus(OK);
	getSubject()->subscribe(_fd, POLLIN, this);
	return 0;
}

int File::httpPost()
{
	bool is_created = false;
	if (execStat() == -1) {
		if (errno == ENOENT) {
			if (createFile() == false) {
				return -1;
			}
		} else if(errno == EACCES) {
			entrustCreateResponse(FORBIDDEN);
			return -1;
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
			return -1;
		}
	} else if (appendFile() == false ){
		return -1;
	}
	if (_fd == -1) {
		perror("open");
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return -1;
	}
	getSubject()->subscribe(_fd, POLLOUT, this);
	return 0;
}

bool File::createFile()
{
	if (execStatForParentDir() == -1) {
		if (errno == ENOENT) {
			entrustCreateResponse(NOT_FOUND);
		} else if(errno == EACCES) {
			entrustCreateResponse(FORBIDDEN);
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
		}
		return false;
	}
	if (checkPermission(S_IWUSR) == false) {
		setHTTPStatus(FORBIDDEN);
		return false;
	}
	_fd = open(getPath().c_str(), O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC, S_IRWXU | S_IRWXG | S_IRWXO);
	setHTTPStatus(CREATED);
	return true;
}

bool File::appendFile()
{
	if (isRegularFile() == false) {
		entrustCreateResponse(FORBIDDEN);
		return false;
	}
	if (checkPermission(S_IWUSR) == false) {
		entrustCreateResponse(FORBIDDEN);
		return false;
	}
	_fd = open(getPath().c_str(), O_RDWR | O_APPEND | O_CLOEXEC);
	setHTTPStatus(OK);
	return true;
}

int File::httpDelete()
{
	if (execStatForParentDir() == -1) {
		if (errno == ENOENT) {
			entrustCreateResponse(NOT_FOUND);
		} else if(errno == EACCES) {
			entrustCreateResponse(FORBIDDEN);
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
		}
		return -1;
	}
	if (checkPermissionOfParent(S_IXOTH) == false) {
		entrustCreateResponse(FORBIDDEN);
		return -1;
	}
	if (unlink(_path.c_str()) == -1) {
		if (errno == EACCES) {
			entrustCreateResponse(UNAUTHORIZED);
		} else if (errno == EBUSY) {
			entrustCreateResponse(CONFLICT);
		} else if (errno == EISDIR ||
					errno == EPERM) {
			entrustCreateResponse(FORBIDDEN);
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
