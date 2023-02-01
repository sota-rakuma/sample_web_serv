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

HTTPRequestParser &HTTPRequestParser::setState(
	RequestParsingState s
)
{
	_state = s;
	return *this;
}

HTTPRequestParser & HTTPRequestParser::setRequest(
	HTTPRequest * req
)
{
	_req = req;
	return *this;
}

int HTTPRequestParser::parse(const std::string & raw)
{
	if (_state == REQUEST_LINE) {
		for (size_t i = 0; i + 2 < raw.size(); i+=2)
		{
			if (raw[i] != '\x0d' || raw[i + 1] != '\x0a') {
				break;
			}
		}

		return parseRequestLine(raw);
	}
	return parseHeaderField(raw);
}

int HTTPRequestParser::parseRequestLine(
	const std::string & raw
)
{
	size_t first_sp = raw.find(' ');
	size_t second_sp = raw.find(' ', first_sp + 1);
	if (first_sp == std::string::npos || second_sp == std::string::npos) {
		return -1;
	}
	const std::string & method = raw.substr(0, first_sp);
	if (parseMethod(method) == -1) {
		return -1;
	}
	first_sp += 1;
	const std::string & target = raw.substr(first_sp, second_sp - first_sp);
	if (parseRequestTarget(target) == -1) {
		return -1;
	}
	const std::string & version = raw.substr(second_sp + 1);
	if (parseHTTPVersion(version) == -1) {
		return -1;
	}
	if (_tp.getQuery().size() > 0) {
		_req->setRequestLine(method, _tp.getPath() + "?" + _tp.getQuery(), version);
	} else {
		_req->setRequestLine(method, _tp.getPath(), version);
	}
	_state = HEADER_FIELD;
	return 0;
}

int HTTPRequestParser::parseMethod(
	const std::string & raw
)
{
	if (isToken(raw) == false) {
		return -1;
	}
	return 0;
}

int HTTPRequestParser::parseRequestTarget(
	const std::string & raw
)
{
	if (_tp.parse(raw) == -1) {
		return -1;
	}
	if (*(raw.begin()) != '/') {
		return _tp.getScheme() == "http" && _tp.getAuthority().size() > 0 ? 0 : -1;
	}
	return 0;
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
	} else if (raw[6] != '.') {
		return -1;
	}
	else if (isDigit(raw[7])== false) {
		return -1;
	}
	return 0;
}

static void allignValue(
	std::string & target,
	const std::string & raw,
	size_t first,
	size_t last
)
{
	first = raw.find_first_not_of("\t ", first);
	if (first == std::string::npos) {
		target = "";
		return ;
	}
	last = raw.find_last_not_of("\t ", last);

	target = raw.substr(first, last - first + 1);

	first = target.find_first_of("\t ");
	while (first != std::string::npos)
	{
		last = target.find_first_not_of("\t ", first);
		if (last == std::string::npos) {
			last = target.size();
		}
		if (last - first > 1) {
			target.replace(first, last - first, " ");
		}
		first = target.find_first_of("\t ", last);
	}
}

int HTTPRequestParser::parseHeaderField(
	const std::string & raw
)
{
	size_t first = 0;
	size_t last = raw.find("\r\n");
	std::string name;

	while (first != last)
	{
		size_t colon = raw.find(':', first);
		std::string val;
		if (last < colon) {
			if (raw[first] != ' ' && raw[first] != '\t') {
				return -1;
			} else {
				_req->setObsFold(true);
				colon = first;
			}
		} else {
			strToLow(name, raw.substr(first, colon - first));
		}
		allignValue(val, raw, colon + 1, last - 1);
		if (parseHeaderName(name) == -1 || parseHeaderValue(val) == -1) {
			return -1;
		}
		_req->insertHeaderField(name, val);
		first = last + 2;
		last = raw.find("\r\n", first);
	}
	_state = REQUEST_LINE;
	return 0;
}

int HTTPRequestParser::parseHeaderName(
	const std::string & raw
)
{
	if (isToken(raw) == false) {
		return -1;
	}
	return 0;
}

int HTTPRequestParser::parseHeaderValue(
	const std::string & raw
)
{
	for (size_t i = 0; i < raw.size(); i++)
	{
		if (raw[i] > 0 && isPrintable(raw[i]) == false) {
			return -1;
		}
	}
	return 0;
}

bool HTTPRequestParser::isToken(
	const std::string & raw
)
{
	if (raw.size() == 0) {
		return false;
	}
	for (size_t i = 0; i < raw.size(); i++) {
		if (isTchar(raw[i]) == false) {
			return false;
		}
	}
	return true;
}

bool HTTPRequestParser::isTchar(char c)
{
	const std::string pchar_symbols("!#$%&'*+-.^_`|~");
	return isDigit(c) || isAlpha(c) ||
			pchar_symbols.find(c) != std::string::npos;
}
