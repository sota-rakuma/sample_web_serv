#include "HTTPRequest.hpp"
#include "utils/utils.hpp"
#include <vector>

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
:_target(target),
_version(version)
{
	if (method == "GET") {
		_method = GET;
	} else if (method == "POST") {
		_method = POST;
	} else if (method == "DELETE") {
		_method = DELETE;
	}
}

HTTPRequest::RequestLine::RequestLine(
	HTTPMethod method,
	const std::string & target,
	const std::string & version
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

HTTPMethod
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
	HTTPMethod method
)
{
	_method = method;
	return *this;
}

HTTPRequest::RequestLine &
HTTPRequest::RequestLine::setMethod(
	const std::string & method
)
{
	if (method == "GET") {
		_method = GET;
	} else if (method == "POST") {
		_method = POST;
	} else if (method == "DELETE") {
		_method = DELETE;
	}
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
HTTPRequest::RequestLine::operator=(const RequestLine & rhs)
{
	_method = rhs._method;
	_target = rhs._target;
	_version = rhs._version;
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
	_reql.setMethod(method)
	.setTarget(target)
	.setHTTPVersion(version);
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

std::ostream &operator<<(
	std::ostream & os,
	const HTTPRequest & req
)
{
	os << addColorText("HTTPREQUEST", GREEN) << std::endl;

	const HTTPRequest::RequestLine & reql = req.getRequestLine();
	os << "REQUEST LINE" << std::endl
	<< "Method: " << addColorText(reql.getMethod(), BLUE) << std::endl
	<< "Target: " << reql.getTarget() << std::endl
	<< "HTTPVersion: " << reql.getHTTPVersion() << std::endl << std::endl;

	const std::map<std::string, std::string> & hf = req.getHeaderField();
	for (std::map<std::string, std::string>::const_iterator it = hf.begin();
		it != hf.end();
		it++)
	{
		os << "" << it->first << ": " << it->second << std::endl;
	}
	os << std::endl;

	os << "Message" << std::endl
	<< req.getMessageBody();

	return os;
}