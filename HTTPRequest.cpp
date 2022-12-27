#include "HTTPRequest.hpp"

HTTPRequest::RequestLine::RequestLine()
:_method(),
_target(),
_version()
{
}

HTTPRequest::RequestLine::RequestLine(
	const std::string &method,
	const std::string &target,
	const std::string &version
)
:_method(method),
_target(target),
_version(version)
{
}

HTTPRequest::RequestLine::RequestLine(
	const RequestLine & another
)
:_method(another._method),
_target(another._target),
_version(another._version)
{
}

HTTPRequest::RequestLine::~RequestLine()
{
}

const std::string &
HTTPRequest::RequestLine::getMethod() const
{
	return _method;
}

const std::string &
HTTPRequest::RequestLine::getTarget() const
{
	return _target;
}

const std::string &
HTTPRequest::RequestLine::getHTTPVersion() const
{
	return _version;
}

HTTPRequest::RequestLine &
HTTPRequest::RequestLine::setMethod(
	const std::string & method
)
{
	_method = method;
	return *this;
}

HTTPRequest::RequestLine &
HTTPRequest::RequestLine::setTarget(
	const std::string & target
)
{
	_target = target;
	return *this;
}

HTTPRequest::RequestLine &
HTTPRequest::RequestLine::setHTTPVersion(
	const std::string & version
)
{
	_version = version;
	return *this;
}

HTTPRequest::RequestLine &
HTTPRequest::RequestLine::operator=(const RequestLine & another)
{
	_method = another._method;
	_target = another._target;
	_version = another._version;
	return *this;
}

HTTPRequest::HTTPRequest()
{
}

HTTPRequest::HTTPRequest(
	const HTTPRequest & another
)
:_reql(another._reql),
_hf(another._hf),
_body(another._body)
{
}

HTTPRequest::~HTTPRequest()
{
}

const HTTPRequest::RequestLine & HTTPRequest::getRequestLine() const
{
	return _reql;
}

const std::map<std::string, std::string> &
HTTPRequest::getHeaderField() const
{
	return _hf;
}

const std::string &
HTTPRequest::getHeaderValue(
	const std::string & key
) const
{
	return _hf.at(key);
}

const std::string &
HTTPRequest::getMessageBody() const
{
	return _body;
}

HTTPRequest &
HTTPRequest::setRequestLine(
	const std::string & method,
	const std::string & target,
	const std::string & version
)
{
	_reql.setMethod(method);
	_reql.setTarget(target);
	_reql.setHTTPVersion(version);
	return *this;
}

HTTPRequest &
HTTPRequest::setHeaderField(
	const std::map<std::string, std::string> & hf
)
{
	_hf = hf;
	return *this;
}

HTTPRequest &
HTTPRequest::insertHeaderField(
	const std::pair<std::string, std::string> & item
)
{
	_hf.insert(item);
	return *this;
}

HTTPRequest &
HTTPRequest::setMessageBody(
	const std::string & body
)
{
	_body = body;
	return *this;
}
