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
	int oflag
)
:HTTPMethodReceiver(subject, commands),
_read(new Read(this)),
_write(new Write(this)),
_path(path),
_is_exist(false)
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
	int mode
)
:HTTPMethodReceiver(subject, commands),
_read(new Read(this)),
_write(new Write(this)),
_path(path),
_is_exist(false)
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
	// 例外処理
	if (event == IN) {
		getCommandList()->push_back(_read);
	} else {
		getCommandList()->push_back(_write);
	}
}

int File::read()
{
	char buff[BUFSIZE];
	ssize_t nb = ::read(_fd, buff, BUFSIZE);
	if (nb < 0) {
		return -1;
	} else if (nb == 0) {
		for (std::list<AcceptedSocket *>::iterator it = _as.begin();
			it != _as.end();
			it++)
		{
			//(*it)->createResponse(OK, _header, _buff);
		}
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	return 1;
}

int File::httpGet()
{
	getSubject()->subscribe(_fd, IN, this);
}
