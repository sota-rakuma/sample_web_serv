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
:_is_exist(false)
{
}

File::File(
	ISubject * subject,
	std::list<ICommand *> * commands,
	HTTPMethod *method,
	const std::string & path,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands, method, as, path),
_nb(0),
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
:HTTPMethodReceiver(subject, commands, as, path),
_nb(0),
_is_exist(false)
{
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
:HTTPMethodReceiver(subject, commands, as, path),
_nb(0),
_is_exist(false)
{
}

File::File(
	ISubject * subject,
	std::list<ICommand *> * commands,
	HTTPMethod *method,
	const std::string & path,
	int oflag,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands, method, as, path),
_nb(0),
_is_exist(false)
{
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
	HTTPMethod *method,
	const std::string & path,
	int oflag,
	int mode,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands, method, as, path),
_nb(0),
_is_exist(false)
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
	// autoindex ha?
	if (checkPermission(S_IROTH) == false) {
		getAcceptedSocket()->setStatus(FORBIDDEN);
		return -1;
	}
	_fd = open(getPath().c_str(), O_RDONLY | O_CLOEXEC);
	if (_fd == -1) {
		getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
		return -1;
	}
	getSubject()->subscribe(_fd, POLLIN, this);
	return 0;
}

// writeした後にファイルの内容を取得
int File::httpPost()
{
	if (checkPermission(S_IWOTH) == false) {
		getAcceptedSocket()->setStatus(FORBIDDEN);
		return -1;
	}
	if (isDirectory() == true) {
// content-typeから拡張子を
		_fd = open(getPath().c_str(), O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC);
	} else {
		_fd = open(getPath().c_str(), O_WRONLY | O_APPEND | O_CLOEXEC);
	}
	if (_fd == -1) {
		getAcceptedSocket()->setStatus(INTERNAL_SERVER_ERROR);
		//_as->createResponse();
		return -1;
	}
	_buff = "value=aaaa&value_2=bbbb";
	getSubject()->subscribe(_fd, POLLOUT, this);
	return 0;
}

int File::httpDelete()
{
	if (unlink(getPath().c_str()) == -1) {
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
