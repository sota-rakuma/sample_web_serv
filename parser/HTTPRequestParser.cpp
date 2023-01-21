#include "HTTPRequestParser.hpp"
#include "../utils/utils.hpp"

HTTPRequestParser::HTTPRequestParser()
:_req(static_cast<HTTPRequest *>(NULL)),
_tp(),
_state(REQUEST_LINE)
{
}

HTTPRequestParser::HTTPRequestParser(HTTPRequest * req)
:_req(req),
_tp(),
_state(REQUEST_LINE)
{
}

HTTPRequestParser::HTTPRequestParser(
	const HTTPRequestParser &another
)
:_req(another._req),
_tp(another._tp),
_state(another._state)
{
}

HTTPRequestParser::~HTTPRequestParser()
{
}

HTTPRequest *&HTTPRequestParser::getRequest()
{
	return _req;
}

RequestParsingState HTTPRequestParser::getState() const
{
	return _state;
}

int HTTPRequestParser::parse(const std::string & raw)
{
	if (_state == REQUEST_LINE) {
		size_t start = 0;
		for (size_t i = 0; i + 2 < raw.size(); i+=2)
		{
			if (raw[i] != '\x0d' || raw[i + 1] != '\x0a') {
				start = i + 2;
				break;
			}
		}

		return parseRequestLine(raw.substr(start));
	}
	return parseHeaderField(raw);
}

int HTTPRequestParser::parseRequestLine(
	const std::string & raw
)
{
	size_t first_sp = raw.find(' ');
	size_t second_sp = raw.find(first_sp + 1, ' ');
	if (first_sp == std::string::npos || second_sp == std::string::npos) {
		return -1;
	}
	const std::string & method = raw.substr(0, first_sp);
	if (parseMethod(method) == -1) {
		return -1;
	}
	const std::string & target = raw.substr(first_sp, second_sp - first_sp);
	if (parseRequestTarget(target) == -1) {
		return -1;
	}
	const std::string & version = raw.substr(second_sp);
	if (parseHTTPVersion(version) == -1) {
		return -1;
	}
	_req->setRequestLine(method, target, version);
	return 0;
}

int HTTPRequestParser::parseMethod(
	const std::string & raw
)
{
	return isToken(raw);
}

int HTTPRequestParser::parseRequestTarget(
	const std::string & raw
)
{
	return _tp.parse(raw);
}

int HTTPRequestParser::parseHTTPVersion(
	const std::string & raw
)
{
	if (raw.size() != 8) {
		return -1;
	}
	if (raw.substr(0, 4) != "HTTP") {
		return -1;
	} else if (raw[4] != '/') {
		return -1;
	} else if (isDigit(raw[5]) == false) {
		return -1;
	} else if (raw[5] != '.') {
		return -1;
	}
	else if (isDigit(raw[7])== false) {
		return -1;
	}
	return 0;
}

/*
OWSは除去

*/
int HTTPRequestParser::parseHeaderField(
	const std::string & raw
)
{
}

bool HTTPRequestParser::isToken(
	const std::string & raw
)
{
	if (raw.size() == 0) {
		return false;
	}
	for (size_t i = 0; i < raw.size(); i++) {
		if (isTcahr(raw[i]) == false) {
			return false;
		}
	}
	return true;
}

/*
 "!" / "#" / "$" / "%" / "&"/"'"/"*"/"+"/"-"/"."/"^"/"_"/"`"/"|"/"~"/ DIGIT / ALPHA
*/
bool HTTPRequestParser::isTcahr(char c)
{
	const std::string pchar_symbols("!#$%&'*+-.^_`|~");
	return isDigit(c) || isAlpha(c) ||
			pchar_symbols.find(c) != std::string::npos;
}
