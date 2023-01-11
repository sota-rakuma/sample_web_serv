#include "Context.hpp"

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
	return _p->parse(tokens);
}