#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Parser.hpp"
#include <cstddef>

class Context {
private:
	Parser *_p; // stateになる。
public:
	Context();
	Context(Parser *);
	~Context();
	void transitionTo(Parser *);
	ParsingStatus execParse(
		const std::vector<std::pair<Symbol, std::string> > &
	);
};

#endif /* CONTEXT_HPP */
