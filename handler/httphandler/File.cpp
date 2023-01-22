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
:_is_exist(false)
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
_as(as)
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
_as(as)
{
	_fd = open(_path.c_str(), oflag | O_CLOEXEC, mode);
	if (_fd == -1) {
		throw FileError("open");
	}
}

File::File(
	ISubject * subject,
	std::list<ICommand *> * commands,
	ICommand *method,
	const std::string & path,
	int oflag,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands, method),
_path(path),
_nb(0),
_is_exist(false),
_as(as)
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
	ICommand *method,
	const std::string & path,
	int oflag,
	int mode,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands, method),
_path(path),
_nb(0),
_is_exist(false),
_as(as)
{
	_fd = open(_path.c_str(), oflag | O_CLOEXEC, mode);
	if (_fd == -1) {
		throw FileError("open");
	}
}

File::~File()
{
	::close(_fd);
}

void File::update(int event)
{
	if (event & (POLLHUP | POLLERR | POLLNVAL)) {
		_as->createResponse("error");
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
		_as->createResponse(_buff);
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
	_as->createResponse("created");
	return 0;
}

int File::httpGet()
{
	getSubject()->subscribe(_fd, POLLIN, this);
	return 0;
}

// writeした後にファイルの内容を取得
int File::httpPost()
{
	_buff = "value=aaaa&value_2=bbbb";
	getSubject()->subscribe(_fd, POLLOUT, this);
	return 0;
}

#include <sys/stat.h>

int File::httpDelete()
{
	int file_fd;
	// ファイルのpathをセット
	std::string file_path = "confファイルから取得"; //
	// ファイルの存在を確認
	if (access(file_path.c_str(), F_OK) == 0)
	{
		// ファイルが存在する場合
		file_fd = open(file_path.c_str(), O_RDWR);
		if (file_fd == -1)
			//open失敗
			_status = INTERNAL_SERVER_ERROR;
		else
		{
			// open成功
			struct stat sb;
			if (fstat(file_fd, &sb) == -1)
				// fstat失敗
				_status = INTERNAL_SERVER_ERROR;
			else
			{
				// fstat成功
				if (sb.st_mode & S_IWOTH)
				{
					// 削除権限がある場合
					if (unlink(file_path.c_str()) == -1)
						// unlink失敗
						_status = INTERNAL_SERVER_ERROR;
					else
						// unlink成功
						_status = OK;
				}
				else
					// 削除権限がない場合
					_status = FORBIDDEN;
				}
			}
	}
	else
		// ファイルが存在しない場合
		_status = NOT_FOUND; // status合ってる？？
	close(file_fd);
	getSubject()->subscribe(_fd, POLLOUT, this);
	return 0;
}

int File::getFd() const {
	return _fd;
}

const std::string & File::getPath() const
{
	return _path;
}