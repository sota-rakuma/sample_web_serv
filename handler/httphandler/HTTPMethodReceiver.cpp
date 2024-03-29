#include "../AcceptedSocket.hpp"
#include "../../command/HTTPMethod.hpp"
#include "HTTPMethodReceiver.hpp"
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
_state(another._state),
_parent(another._parent),
_path(another._path),
_query(another._query),
_buff(another._buff)
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject * sub,
	std::list<ICommand *> *commands,
	AcceptedSocket * as
)
:IOEventHandler(sub, commands),
_method(static_cast<HTTPMethod *>(NULL)),
_as(as),
_path(),
_query(),
_buff()
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
_path(path),
_buff()
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject *subject,
	std::list<ICommand *> *commands,
	AcceptedSocket *as,
	const std::string &path,
	const std::string &query
)
:IOEventHandler(subject, commands),
_method(static_cast<HTTPMethod *>(NULL)),
_as(as),
_path(path),
_query(query),
_buff()
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
_path(path),
_buff()
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject *subject,
	std::list<ICommand *> *commands,
	HTTPMethod *method,
	AcceptedSocket *as,
	const std::string &path,
	const std::string & query
)
:IOEventHandler(subject, commands),
_method(method),
_as(as),
_path(path),
_query(query),
_buff()
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject *subject,
	std::list<ICommand *> *commands,
	HTTPMethod *method,
	AcceptedSocket *as,
	const std::string &path,
	const std::string &query,
	const std::string &buff
)
:IOEventHandler(subject, commands),
_method(method),
_as(as),
_path(path),
_query(query),
_buff(buff)
{
}

HTTPMethodReceiver::~HTTPMethodReceiver()
{
	for (std::list<ICommand *>::iterator it = getCommandList()->begin();
		it != getCommandList()->end();)
	{
		if (*it == _method) {
			getCommandList()->erase(it++);
		} else {
			++it;
		}
	}
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

const std::string & HTTPMethodReceiver::getQuery() const
{
	return _query;
}

void HTTPMethodReceiver::setHTTPMethod(
	HTTPMethod *method
)
{
	if (method != static_cast<HTTPMethod *>(NULL)) {
		delete _method;
	}
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

void HTTPMethodReceiver::setQuery(
	const std::string & q
)
{
	_query = q;
}

void HTTPMethodReceiver::setContent(
	const std::string & content
)
{
	_buff = content;
}

void HTTPMethodReceiver::addContent(
	const std::string & content
)
{
	_buff += content;
}

const std::string & HTTPMethodReceiver::getContent() const
{
	return _buff;
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

int HTTPMethodReceiver::execStatForParentDir()
{
	const std::string & parent = _path.substr(0, _path.rfind('/'));
	return stat(parent.c_str(), &_parent);
}

bool HTTPMethodReceiver::checkPermissionOfParent(mode_t perm) const
{
	return _parent.st_mode & perm;
}

void HTTPMethodReceiver::setHTTPStatus(
	HTTPStatus status
)
{
	_as->setStatus(status);
}

void HTTPMethodReceiver::entrustCreateResponse(
	HTTPStatus status
)
{
	int s = static_cast<int>(status);
	if (400 <= s && s < 600) {
		return _as->createErrorResponse(status);
	}
	_as->setStatus(status).createNormalResponse();
}

void HTTPMethodReceiver::entrustCreateResponse()
{
	_as->createNormalResponse();
}
