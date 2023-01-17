#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

class Parser
{
private:
	/* data */
public:
	Parser(/* args */);
	virtual ~Parser();
	virtual int parse(
		const std::string &
	) = 0;
};

#endif /* PARSER_HPP */
