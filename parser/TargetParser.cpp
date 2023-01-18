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

	size_t start = 0;
	size_t index = raw.find(':');
	if (index == std::string::npos) {
		return -1;
	}
	if (parseScheme(raw.substr(start, index - start)) == false) {
		std::cout << "SCHEME ERROR" << std::endl;
		return -1;
	}
	if (raw[index + 1] != '/' || raw[index + 2] != '/') {
		return -1;
	}

	start = index + 3;
	index = raw.find_first_of("/?#", start);
	if (parseAuthority(raw.substr(start, index - start)) == false) {
		std::cout << "AUTHORITY ERROR" << std::endl;
		return -1;
	}
	if (index == std::string::npos) {
		return 0;
	}

	start = index;
	index = raw.find_first_of("?#", start);
	if (parsePath(raw.substr(start, index - start)) == false) {
		std::cout << "PATH ERROR" << std::endl;
		return -1;
	}
	if (index == std::string::npos) {
		return 0;
	}

	start = index + 1;
	if (raw[index] == '?') {
		index = raw.find('#', start);
		const std::string & q = raw.substr(start, index - start);
		if (parseQueryFragment(q) == false)
		{
			std::cout << "QUERY ERROR" << std::endl;
			return -1;
		}
		_query = q;
		if (index == std::string::npos) {
			return 0;
		}
		start = index + 1;
	}
	if (raw[index] == '#') {
		const std::string & frag = raw.substr(start);
		if (parseQueryFragment(frag) == false)
		{
			std::cout << "FRAGMENT ERROR" << std::endl;
			return -1;
		}
		_fragment = frag;
	}
	return 0;
}

int TargetParser::parseAbsolutePath(
	const std::string & raw
)
{
	if (raw.size() == 0) {
		return false;
	}
	size_t index = raw.find('?');
	if (index == std::string::npos) {
		return parsePath(raw) == true ? 0 : -1;
	}

	if (parsePath(raw.substr(0, index)) == false) {
		return -1;
	}
	const std::string & q = raw.substr(index + 1);
	if (parseQueryFragment(q) == false) {
		return -1;
	}
	_query = q;
	return 0;
}

bool TargetParser::parseScheme(
	const std::string & raw
)
{
	if (isAlpha(raw[0]) == false) {
		return false;
	}
	for (size_t i = 1; i < raw.size(); i++) {
		if (isAlpha(raw[i]) == false && isDigit(raw[i]) == false &&
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
			std::cout << "USERINFO ERROR" << std::endl;
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
		std::cout << "HOST ERROR" << std::endl;
		return false;
	}
	_authority = raw;
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
	if (raw[first] == '[' && raw[last] == ']') {
		if (parseIPv6(first + 1, last, raw) == true) {
			return true;
		}
		return parseIPvF(first + 1, last, raw);
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
	//const std::string &r = raw.substr(first, last - first);
	//std::cout << "raw: " << r << std::endl;

	unsigned char buf[sizeof(in6_addr)];
	std::string ip = raw.substr(first, last - first);
	return inet_pton(AF_INET6, ip.c_str(), buf) == 1;
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
	unsigned char buf[sizeof(in_addr)];
	std::string ip = raw.substr(first, last - first + 1);
	return inet_pton(AF_INET, ip.c_str(), buf) == 1;
}

bool TargetParser::parseRegName(
	size_t first,
	size_t last,
	const std::string & raw
)
{
	while (first < last) {
		if (isUnreserved(raw[first]) == true ||
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
	_path = raw;
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
	return isUnreserved(raw[index]) || isSubDelim(raw[index]) ||
			raw[index] == ':' || raw[index] == '@';
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

#include <vector>

void addNormalCase(
	std::vector<std::string> & test
)
{
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http://127.0.0.1:8080/pub/ietf/uri/#Related");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]:8080");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]:");
	test.push_back("http://[::3:1000:100:20:3]:8080");
	test.push_back("http://[::3:1000:100:127.0.0.1]:8080");
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/?query#Related");
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/?query");
	test.push_back("http://[::3:1000:100:127.0.0.1]:8080?query");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/?query");
}

void addAbnormalCase(
	std::vector<std::string> & test
)
{
	test.push_back("");
	test.push_back("://www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http//www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http:/www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http//www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3");
	test.push_back("http://2001:db8:20:3:1000:100:20:3");
	test.push_back("http:///");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/#Related");
}

int main(void)
{
	std::vector<std::string> test;

// 正常系
	//addNormalCase(test);
// 異常系
	addAbnormalCase(test);


	for (size_t i = 0; i < test.size(); i++)
	{
		TargetParser p;
		std::cout << "test[" << i << "]: " << test[i] << std::endl;
		if (p.parse(test[i]) == -1) {
			std::cout << addColorText("PARSER ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("PARSE SUCCESS", GREEN) << std::endl;
		}
		std::cout << p << std::endl << std::endl;
	}

	return 0;
}
