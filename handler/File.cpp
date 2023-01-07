#include "../observer/IObserver.hpp"
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
_write(new Write(this))
{
}

File::File(
	const std::string & path,
	int oflag
)
:_path(path),
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
 * @param observer event monitor
 * @param name getTarget();
 * @param oflag O_RDWR | O_NONBLOCK | O_CREAT
 */
File::File(
	const std::string & path,
	int oflag,
	int mode
)
:_path(path),
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

ICommand* File::getHandler(int event) const
{
	// 例外処理
	if (event == IN) {
		return _read;
	} else if (event == OUT) {
		return _write;
	}
}