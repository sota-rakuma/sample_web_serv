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
	void setParser(Parser *);
	void transitionTo(Parser *);
	int execParse(
		const std::string &
	);
};

#endif /* CONTEXT_HPP */
