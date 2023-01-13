#ifndef PARSER_HPP
#define PARSER_HPP

#include "SyntaxTree.hpp"

enum ParsingStatus {
	SUCCESS,
	COTINUE,
	ERROR,
};

class Parser
{
private:
	/* data */
public:
	Parser(/* args */);
	virtual ~Parser();
	virtual ParsingStatus parse(
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
