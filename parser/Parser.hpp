#ifndef PARSER_HPP
#define PARSER_HPP

#include "SyntaxTree.hpp"

enum Status {
	SUCCESS,
	ERROR,
};

class Parser
{
private:
	/* data */
public:
	Parser(/* args */);
	~Parser();
	virtual Status parse(
		const std::vector<std::pair<Symbol, std::string> > &
	) = 0;
};

Parser::Parser(/* args */)
{
}

Parser::~Parser()
{
}


#endif /* PARSER_HPP */
