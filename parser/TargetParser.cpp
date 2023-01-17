#include "TargetParser.hpp"
#include "../utils/utils.hpp"
#include <sstream>
#include <arpa/inet.h>

std::string TargetParser::_delims = ":/?#";
std::string TargetParser::_sub_delim = "!$&'()*+,;=";
std::string TargetParser::_gen_delim = ":/?#[]@";

TargetParser::TargetParser()
:_progress(0)
{
}

TargetParser::TargetParser(const TargetParser & another)
:_progress(another._progress)
{
}

TargetParser::~TargetParser()
{
}

int TargetParser::parse(
	const std::string & raw
)
{
	if (raw[0] == '/') {
		return parseAbsolutePath(raw);
	}
	splitRaw(raw);
	size_t start = 0;
	size_t index = raw.find(':');
	if (parseScheme(raw.substr(start, index)) == false)
	{
		return -1;
	}
	start = index + 1;
	index = raw.find('/', start);
	return 0;
}

int TargetParser::parseAbsolutePath(
	const std::string & raw
)
{
	if (raw.size() == 0) {
		return false;
	}
	return parsePath(raw);
}

bool TargetParser::parseScheme(
	const std::string & raw
)
{
	if (isAlpha(raw[0]) == false) {
		return false;
	}
	for (size_t i = 1; i < raw.size(); i++) {
		if (isAlpha(raw[i]) == false || isDigit(raw[i]) == false ||
			(raw[i] != '+' && raw[i] != '-' && raw[i] != '.'))
		{
			return false;
		}
	}
	_scheme = raw;
	return true;
}

bool TargetParser::parseAuthority(
	const std::string & raw
)
{
	size_t first = 0;
	size_t last = raw.size() - 1;
	size_t delim = raw.find('@');
	if (delim != std::string::npos) {
		if (parseUserInfo(raw.substr(0, delim)) == false) {
			return false;
		}
		first = delim + 1;
	}
	size_t index = raw.find_last_of(':');
	if (index != std::string::npos &&
		raw.find_first_not_of("0123456789", index + 1) == std::string::npos) {
		last = index - 1;
	}
	if (parseHost(first, last, raw) == false) {
		return false;
	}
	return true;
}

bool TargetParser::parseUserInfo(
	const std::string & raw
)
{
	for (size_t i = 0; i < raw.size(); i++) {
		if (isUnreserved(raw[i]) || isSubDelim(raw[i]) || raw[i] == ':') {
			continue;
		}
		if (isPercentEncoded(raw, i) == false) {
			return false;
		}
	}
	return true;
}

bool TargetParser::parseHost(
	size_t first,
	size_t last,
	const std::string & raw
)
{
	if (raw[first] == '[' && raw[last] == '[') {
		if (parseIPv6(first, last, raw) == true) {
			return true;
		}
		return parseIPvF(first + 1, last - 1, raw);
	}
	if (parseIPv4(first, last, raw) == true) {
		return true;
	}
	return parseRegName(first, last, raw);
}

bool TargetParser::parseIPv6(
	size_t first,
	size_t last,
	const std::string & raw
)
{
	unsigned char buf[sizeof(in6_addr)];
	std::string ip = raw.substr(first, last - first);
	return inet_pton(AF_INET6, ip.c_str(), buf);
}

bool TargetParser::parseIPvF(
	size_t first,
	size_t last,
	const std::string & raw
)
{
	if (raw[first] != 'v' && isHexDig(raw[++first]) == false) {
		return false;
	}
	while (isHexDig(raw[first]))
	{
		first++;
	}
	if (first == last) {
		return false;
	}
	while (first < last) {
		if (isUnreserved(raw[first]) == false &&
			isSubDelim(raw[first]) == false &&
			raw[first] != ':')
		{
			return false;
		}
	}
	return true;
}

bool TargetParser::parseIPv4(
	size_t first,
	size_t last,
	const std::string & raw
)
{
	size_t cnt = 0;
	while (first < last) {
		if (cnt > 4) {
			return false;
		}
		size_t dot = raw.find('.', first);
		if (dot == std::string::npos) {
			dot = last;
		}

		size_t dec_octet = 0;
		while (first < dot) {
			if (isDigit(raw[first]) == false) {
				return false;
			}
			dec_octet = dec_octet * 10 +  (raw[first] - '0');
			first++;
		}
		if ( dec_octet > 255) {
			return false;
		}
		first++;
	}
	return cnt == 4;
}

bool TargetParser::parseRegName(
	size_t first,
	size_t last,
	const std::string & raw
)
{
	while (first < last) {
		if (isUnreserved(raw[first]) == true &&
			isSubDelim(raw[first]) == true)
		{
			first++;
			continue;
		}
		if (isPercentEncoded(raw, first) == false) {
			return false;
		}
		first += 2;
	}
	return true;
}

bool TargetParser::parsePath(
	const std::string & raw
)
{
	for (size_t i = 0; i < raw.size();)
	{
		if (raw[i] != '/') {
			return false;
		}
		i++;
		while (isPchar(raw, i))
		{
			if (raw[i] == '%') {
				i += 2;
			}
			i++;
		}
	}
	return true;
}

// 外で代入
bool TargetParser::parseQueryFragment(
	const std::string & raw
)
{
	for (size_t i = 0; i < raw.size(); i++) {
		if (isPchar(raw, i) == true) {
			if (raw[i] == '%') {
				i += 2;
			}
			continue;
		}
		if (raw[i] != '/' && raw[i] != '?') {
			return false;
		}
	}
	return true;
}

bool TargetParser::isPchar(
	const std::string & raw,
	size_t index
)
{
	if (isPercentEncoded(raw, index) == true) {
		return true;
	}
	if (raw.size() != 1) {
		return false;
	}
	return isUnreserved(raw[index]) || isSubDelim(raw[index]) ||
			raw == ":" || raw == "@";
}

bool TargetParser::isPercentEncoded(
	const std::string & target,
	size_t index
)
{
	if (target.size() < index + 2) {
		return false;
	}
	return target[index] == '%' &&
			isHexDig(target[index + 1]) && isHexDig(target[index + 2]);
}

bool TargetParser::isUnreserved(
	char c
)
{
	return _gen_delim.find(c) == std::string::npos &&
			_sub_delim.find(c) == std::string::npos;
}

bool TargetParser::isSubDelim(
	char c
)
{
	return _sub_delim.find(c) != std::string::npos;
}

void TargetParser::splitRaw(const std::string & raw)
{
	size_t index = 0;
	index = splitComponent(index, raw, _scheme);
	if (raw[index] == ':') {
		index += 1;
	}
	if (raw.find("//") == index)
	{
		index += 2;
		index = splitComponent(index, raw, _authority);
	}
	index = splitComponent(index, raw, _path);
	if (raw[index] == '?') {
		index += 1;
		index = splitComponent(index, raw, _query);
	}
	if (raw[index] == '#') {
		index += 1;
		index = splitComponent(index, raw, _fragment);
	}
	_progress = 0;
}

size_t TargetParser::splitComponent(
	size_t start,
	const std::string & raw,
	std::string & target
)
{
	size_t index = start;
	bool f = true;
	for (; index < raw.size(); index++) {
		for (size_t i = _progress; i < _delims.size(); i++) {
			if (raw[index] == _delims[i]) {
				f = false;
				break ;
			}
		}
		if (f == false) {
			break;
		}
	}
	if (index < 2 && raw[index] != _delims[_progress]) {
		_progress += 1;
		return start;
	}
	if ((_progress == 0 && index - start < 1) ||
			(_progress > 0 && index - start == 0))
	{
		_progress += 1;
		return start;
	}
	target = raw.substr(start, index - start);
	_progress += 1;
	return index;
}


const std::string & TargetParser::getScheme() const
{
	return _scheme;
}

const std::string & TargetParser::getAuthority() const
{
	return _authority;
}

const std::string & TargetParser::getPath() const
{
	return _path;
}

const std::string & TargetParser::getQuery() const
{
	return _query;
}

const std::string & TargetParser::getFragment() const
{
	return _fragment;
}

std::ostream& operator<<(std::ostream & os, const TargetParser & p)
{
	os << "Scheme: " << p.getScheme() << std::endl
	<< "Authority: " << p.getAuthority() << std::endl
	<< "Path: " << p.getPath() << std::endl
	<< "Query: " << p.getQuery() << std::endl
	<< "Fragment: " << p.getFragment();

	return os;
}

int main(void)
{
	std::string test = "http://www.ics.uci.edu/pub/ietf/uri/#Related";

	TargetParser p;
	p.parse(test);
	std::cout << "test: " << test << std::endl;
	std::cout << p << std::endl;
	return 0;
}
