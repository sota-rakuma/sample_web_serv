#include "Context.hpp"
#include <stdexcept>

Context::Context()
:_p(static_cast<Parser *>(NULL))
{
}

Context::Context(Parser *p)
:_p(p)
{
};

Context::~Context(){
	delete _p;
}

void Context::transitionTo(Parser *p)
{
	if (_p != static_cast<Parser *>(NULL)) {
		delete _p;
	}
	_p = p;
}

ParsingStatus Context::execParse(
	const std::vector<std::pair<Symbol, std::string> > & tokens
)
{
	if (_p == static_cast<Parser *>(NULL)) {
		throw std::runtime_error("Parser is not set");
	}
	return _p->parse(tokens);
}
