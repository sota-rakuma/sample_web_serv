#include "HTTPResponse.hpp"
#include "../utils/utils.hpp"
#include "../utils/Template.hpp"
#include <iostream>

static std::pair<HTTPStatus, const std::string> temp[] = {std::make_pair(OK, "OK")};

std::map<HTTPStatus, const std::string &> HTTPResponse::_err_msg(temp, temp + getSize(temp));

HTTPResponse::StatusLine::StatusLine()
:_version(),
_code(),
_reason()
{
}

HTTPResponse::StatusLine::StatusLine(
	const std::string & version,
	HTTPStatus code,
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

HTTPStatus HTTPResponse::StatusLine::getCode() const
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

HTTPResponse::StatusLine & HTTPResponse::StatusLine::setCode(HTTPStatus code)
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
	_statl.setCode(status);
	return *this;
}

HTTPResponse & HTTPResponse::setStatusLine(
	const std::string & version,
	HTTPStatus code,
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

HTTPResponse & HTTPResponse::setMessageBody(
	const std::string & body
)
{
	_body = body;
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

	if (statl.getCode() < 200) {
		os << addColorText(statl.getCode(), BLUE);
	} else if (statl.getCode() < 300) {
		os << addColorText(statl.getCode(), GREEN);
	} else if (statl.getCode() < 400) {
		os << statl.getCode();
	} else {
		os << addColorText(statl.getCode(), RED);
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