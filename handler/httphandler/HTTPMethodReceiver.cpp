#include "../AcceptedSocket.hpp"
#include "HTTPMethodReceiver.hpp"
#include "../../HTTP/HTTPStatusCode.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

HTTPMethodReceiver::HTTPMethodReceiver()
{
}

HTTPMethodReceiver::HTTPMethodReceiver(const HTTPMethodReceiver & another)
:IOEventHandler(another.getSubject(), another.getCommandList()),
_method(another._method),
_as(another._as),
_path(another._path),
_state(another._state)
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject *subject,
	std::list<ICommand *> *commands,
	AcceptedSocket *as,
	const std::string &path
)
:IOEventHandler(subject, commands),
_method(static_cast<HTTPMethod *>(NULL)),
_as(as),
_path(path)
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject *subject,
	std::list<ICommand *> *commands,
	HTTPMethod *method,
	AcceptedSocket *as,
	const std::string &path
)
:IOEventHandler(subject, commands),
_method(method),
_as(as),
_path(path)
{
}

HTTPMethodReceiver::~HTTPMethodReceiver()
{
	delete _method;
}

HTTPMethod *HTTPMethodReceiver::getHTTPMethod() const
{
	return _method;
}

AcceptedSocket *HTTPMethodReceiver::getAcceptedSocket() const
{
	return _as;
}

const std::string & HTTPMethodReceiver::getPath() const
{
	return _path;
}

void HTTPMethodReceiver::setHTTPMethod(
	HTTPMethod *method
)
{
	_method = method;
}

void HTTPMethodReceiver::setAcceptedSocket(
	AcceptedSocket * as
)
{
	_as = as;
}

void HTTPMethodReceiver::setPath(const std::string & path)
{
	_path = path;
}

int HTTPMethodReceiver::execStat()
{
	return stat(_path.c_str(), &_state);
}

bool HTTPMethodReceiver::isDirectory() const
{
	return (_state.st_mode & S_IFMT) & S_IFDIR;
}

bool HTTPMethodReceiver::isRegularFile() const
{
	return (_state.st_mode & S_IFMT) & S_IFREG;
}

bool HTTPMethodReceiver::checkPermission(mode_t perm) const
{
	return _state.st_mode & perm;
}

void HTTPMethodReceiver::entrustCreateResponse(
	HTTPStatus status
)
{
	_as->setStatus(status);
	_as->createResponse();
}