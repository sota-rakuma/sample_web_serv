#ifndef URIPARSER_HPP
#define URIPARSER_HPP

#include "Parser.hpp"
#include <string>

class URIParser : public Parser{
private:
	static std::string _delims;
	size_t _progress;
public:
	URIParser();
	URIParser(const URIParser &);
	~URIParser();
	int parse(const std::string &);
};

#endif /* URIPARSER_HPP */
