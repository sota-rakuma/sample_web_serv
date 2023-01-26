#include "HTTPResponse.hpp"
#include "../utils/utils.hpp"
#include "../utils/Template.hpp"
#include <iostream>
#include <sstream>

static std::pair<HTTPStatus, std::string> temp[] = {
	std::make_pair(OK, "OK"),
	std::make_pair(MOVED_PERMANENTLY, "moved permanently"),
	std::make_pair(FOUND, "Found"),
	std::make_pair(SEE_OTHER, "see other"),
	std::make_pair(NOT_MODIFIED, "not modified"),
	std::make_pair(TEMPORARY_REDIRECT, "Temporary Redirect"),
	std::make_pair(PERMANENT_REDIRECT, "permanent redirect"),
	std::make_pair(CREATED, "Created"),
	std::make_pair(BAD_REQUEST, "Bad Request"),
	std::make_pair(FORBIDDEN, "Forbidden"),
	std::make_pair(NOT_FOUND, "Not Found"),
	std::make_pair(METHOD_NOT_ALLOWED, "Method Not Allowed"),
	std::make_pair(REQUEST_TIMEOUT, "time out"),
	std::make_pair(PAYLOAD_TOO_LARGE, "payload too large"),
	std::make_pair(URI_TOO_LONG, "uri too long"),
	std::make_pair(UNSUPPORTED_MEDIA_TYPE, "unsupported media type"),
	std::make_pair(REQUEST_HEADER_FIELD_TOO_LARGE, "request header filed too large"),
	std::make_pair(INTERNAL_SERVER_ERROR, "Internal Server Error"),
	std::make_pair(NOT_IMPLEMENTED, "not implemented"),
	std::make_pair(HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported")
};

std::map<HTTPStatus, const std::string &> HTTPResponse::_err_msg(temp, temp + getSize(temp));

HTTPResponse::StatusLine::StatusLine()
:_version("HTTP/1.1"),
_code("default"),
_reason()
{
}

HTTPResponse::StatusLine::StatusLine(
	const std::string & version,
	const std::string & code,
	const std::string & reason
)
:_version(version),
_code(code),
_reason(reason)
{
}

HTTPResponse::StatusLine::StatusLine(const StatusLine & another)
:_version(another._version),
_code(another._code),
_reason(another._reason)
{
}

HTTPResponse::StatusLine::~StatusLine()
{
}

const std::string & HTTPResponse::StatusLine::getHTTPVersion() const
{
	return _version;
}

const std::string & HTTPResponse::StatusLine::getCode() const
{
	return _code;
}

const std::string & HTTPResponse::StatusLine::getReason() const
{
	return _reason;
}

HTTPResponse::StatusLine & HTTPResponse::StatusLine::setHTTPVersion(
	const std::string & version
)
{
	_version = version;
	return *this;
}

HTTPResponse::StatusLine & HTTPResponse::StatusLine::setCode(const std::string & code)
{
	_code = code;
	return *this;
}

HTTPResponse::StatusLine & HTTPResponse::StatusLine::setReason(
	const std::string & reason
)
{
	_reason = reason;
	return *this;
}

HTTPResponse::StatusLine & HTTPResponse::StatusLine::operator=(
	const StatusLine & rhs
)
{
	_version = rhs._version;
	_code = rhs._code;
	_reason = rhs._reason;
	return *this;
}

HTTPResponse::HTTPResponse()
:_statl(),
_hf(),
_body()
{
}

HTTPResponse::HTTPResponse(
	const HTTPResponse & another
)
:_statl(another._statl),
_hf(another._hf),
_body(another._body)
{
}

HTTPResponse::~HTTPResponse()
{
}

const HTTPResponse::StatusLine &
HTTPResponse::getStatusLine() const
{
	return _statl;
}

const std::map<std::string, std::string> &
HTTPResponse::getHeaderField() const
{
	return _hf;
}

const std::string & HTTPResponse::getHeaderValue(
	const std::string & key
) const
{
	return _hf.at(key);
}

const std::string & HTTPResponse::getMessageBody() const
{
	return _body;
}

HTTPResponse & HTTPResponse::setStatusCode(
	HTTPStatus status
)
{
	if (_statl.getCode() != "default") {
		return *this;
	}
	std::map<HTTPStatus, const std::string &>::const_iterator reason = _err_msg.find(status);
	std::stringstream ss;
	ss << status;
	_statl.setCode(ss.str());
	ss.clear();
	if (reason == _err_msg.end()) {
		return *this;
	}
	_statl.setReason(reason->second);
	return *this;
}

HTTPResponse & HTTPResponse::setStatusCode(
	const std::string & status
)
{
	if (_statl.getCode() != "default") {
		return *this;
	}
	_statl.setCode(status);
	std::stringstream ss;
	int s;
	ss << status;
	ss >> s;
	if (ss) {
		return *this;
	}
	std::map<HTTPStatus, const std::string &>::const_iterator reason = _err_msg.find(static_cast<HTTPStatus>(s));
	if (reason == _err_msg.end()) {
		return *this;
	}
	_statl.setReason(reason->second);
	return *this;
}

HTTPResponse & HTTPResponse::setStatusCode(
	const std::string & status,
	const std::string & reason
)
{
	if (_statl.getCode() != "default") {
		return *this;
	}
	_statl.setCode(status);
	_statl.setReason(reason);
	return *this;
}

HTTPResponse & HTTPResponse::setStatusLine(
	const std::string & version,
	const std::string & code,
	const std::string & reason
)
{
	_statl.setHTTPVersion(version)
	.setCode(code)
	.setReason(reason);
	return *this;
}

HTTPResponse & HTTPResponse::setHeaderField(
	const std::map<std::string, std::string> & hf
)
{
	_hf = hf;
	return *this;
}

HTTPResponse & HTTPResponse::insertHeaderField(
	const std::pair<std::string, std::string> & item
)
{
	_hf.insert(item);
	return *this;
}

HTTPResponse & HTTPResponse::insertHeaderField(
	const std::string & key,
	const std::string & val
)
{
	_hf.insert(std::make_pair(key, val));
	return *this;
}

HTTPResponse & HTTPResponse::setMessageBody(
	const std::string & body
)
{
	_body = body;
	return *this;
}

HTTPResponse & HTTPResponse::addMessageBody(
	const std::string & body
)
{
	_body += body;
	return *this;
}

std::ostream &operator<<(
	std::ostream & os,
	const HTTPResponse & res
)
{
	os << addColorText("RESPONSE", CYAN) << std::endl;

	const HTTPResponse::StatusLine & statl = res.getStatusLine();
	os << "STATUS LINE" << std::endl
	<< "Version: " << statl.getHTTPVersion() << std::endl
	<< "Code: ";
	std::stringstream ss;
	ss << statl.getCode();
	size_t stat;
	ss >> stat;
	if (ss || (stat <= 400 && stat < 600)) {
		os << addColorText(statl.getCode(), RED);
	} else if (stat < 200) {
		os << addColorText(statl.getCode(), BLUE);
	} else if (stat < 300) {
		os << addColorText(statl.getCode(), GREEN);
	}
	os << std::endl
	<< "Reason: " << statl.getReason() << std::endl;

	const std::map<std::string, std::string> & hf = res.getHeaderField();
	for (std::map<std::string, std::string>::const_iterator it = hf.begin();
		it != hf.end();
		it++)
	{
		os << "" << it->first << ": " << it->second << std::endl;
	}
	os << std::endl;

	os << "Message" << std::endl
	<< res.getMessageBody();

	return os;
}