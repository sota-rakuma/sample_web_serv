#ifndef TARGETPARSER_HPP
#define TARGETPARSER_HPP

#include "Parser.hpp"
#include <iostream>

class TargetParser : public Parser {
private:
	static std::string _delims;
	static std::string _gen_delim;
	static std::string _sub_delim;
	size_t _progress;
	std::string _scheme;
	std::string _authority;
	std::string _path;
	std::string _query;
	std::string _fragment;
private:
	int parseAbsolutePath(
		const std::string &
	);
	bool parseScheme(
		const std::string &
	);
	bool parseAuthority(
		const std::string &
	);
	bool parseUserInfo(
		const std::string &
	);
	bool parseHost(
		size_t,
		size_t,
		const std::string &
	);
	bool parseIPv6(
		size_t,
		size_t,
		const std::string &
	);
	bool parseIPvF(
		size_t,
		size_t,
		const std::string &
	);
	bool parseRegName(
		size_t,
		size_t,
		const std::string &
	);
	bool parseIPv4(
		size_t,
		size_t,
		const std::string &
	);
	bool parsePort(
		size_t,
		const std::string &
	);
	bool parsePath(
		const std::string &
	);
	bool parseQueryFragment(
		const std::string &
	);
public:
	TargetParser();
	TargetParser(const TargetParser &);
	~TargetParser();
	static bool isPchar(
		const std::string &,
		size_t
	);
	static bool isPercentEncoded(
		const std::string &,
		size_t
	);
	static bool isUnreserved(
		char c
	);
	static bool isSubDelim(
		char c
	);
	const std::string & getScheme() const;
	const std::string & getAuthority() const;
	const std::string & getPath() const;
	const std::string & getQuery() const;
	const std::string & getFragment() const;
	virtual int parse(const std::string &);
};

std::ostream &operator<<(std::ostream &, const TargetParser &);

#endif /* TARGETPARSER_HPP */
