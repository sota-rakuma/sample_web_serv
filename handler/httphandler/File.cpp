#include "../AcceptedSocket.hpp"
#include "File.hpp"
#include <fcntl.h>
#include <unistd.h>
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
:_read(new Read(this)),
_write(new Write(this)),
_is_exist(false)
{
}

File::File(
	ISubject * subject,
	std::list<ICommand *> * commands,
	const std::string & path,
	int oflag,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands),
_path(path),
_nb(0),
_is_exist(false),
_as(as),
_read(new Read(this)),
_write(new Write(this))
{
	_fd = open(_path.c_str(), oflag | O_CLOEXEC);
	if (_fd == -1) {
		throw FileError("open");
	}
}

/**
 * @brief Construct a new File:: File object
 *
 * @param oldobserver event monitor
 * @param name getTarget();
 * @param oflag O_RDWR | O_NONBLOCK | O_CREAT
 */
File::File(
	ISubject * subject,
	std::list<ICommand *> * commands,
	const std::string & path,
	int oflag,
	int mode,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands),
_path(path),
_nb(0),
_is_exist(false),
_as(as),
_read(new Read(this)),
_write(new Write(this))
{
	_fd = open(_path.c_str(), oflag | O_CLOEXEC, mode);
	if (_fd == -1) {
		throw FileError("open");
	}
}

File::~File()
{
	delete _read;
	delete _write;
	::close(_fd);
}

void File::update(int event)
{
	if (event & (POLLHUP | POLLERR | POLLNVAL)) {
		_as->createResponse("error");
		getSubject()->unsubscribe(_fd, false, this);
		return ;
	}
	if (event & POLLOUT) {
		getCommandList()->push_back(_write);
	} else if (event == POLLIN) {
		getCommandList()->push_back(_read);
	}
}

int File::read()
{
	char buff[BUFSIZE];
	ssize_t nb = ::read(_fd, buff, BUFSIZE);
	if (nb < 0) {
		return -1;
	} else if (nb == 0) {
		_as->createResponse(_buff);
		getSubject()->unsubscribe(_fd, false, this);
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
		return -1;
	}
	if (nb < _nb + _buff.size()) {
		_buff = _buff.substr(0, nb);
		_nb += nb;
		return 1;
	}
	getSubject()->unsubscribe(_fd, false, this);
	return 0;
}

int File::httpGet()
{
	getSubject()->subscribe(_fd, POLLIN, this);
	return 0;
}

int File::httpPost()
{
	getSubject()->subscribe(_fd, POLLOUT, this);
	return 0;
}

int File::httpDelete()
{
	return 0;
}

int File::getFd() const {
	return _fd;
}

const std::string & File::getPath() const
{
	return _path;
}