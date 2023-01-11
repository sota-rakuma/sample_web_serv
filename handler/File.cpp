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
:_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this)),
_is_exist(false)
{
}

File::File(
	const std::string & path,
	int oflag
)
:_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this)),
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
 * @param observer event monitor
 * @param name getTarget();
 * @param oflag O_RDWR | O_NONBLOCK | O_CREAT
 */
File::File(
	const std::string & path,
	int oflag,
	int mode
)
:_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this)),
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
	delete _get;
	delete _post;
	delete _delete;
	::close(_fd);
}

int File::httpDelete()
{

}

int File::httpPost()
{

}

int File::httpGet()
{
	if (_is_exist == false) {
		notify(_fd, IN, this);
		_is_exist = true;
		return 1;
	}
	char buff[BUFSIZE];
	ssize_t nb = read(_fd, buff, BUFSIZE);
	if (nb < 0) {
		return -1;
	} else if (nb == 0) {
		for (std::list<AcceptedSocket *>::iterator it = _as.begin();
			it != _as.end();
			it++)
		{
			//(*it)->createResponse(200, _header, _buff);
		}
		notify(_fd, DELETE, this);
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	return 1;
}