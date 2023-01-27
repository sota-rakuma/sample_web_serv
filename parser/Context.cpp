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

void Context::setParser(Parser *p)
{
	_p = p;
}

void Context::transitionTo(Parser *p)
{
	if (_p != static_cast<Parser *>(NULL)) {
		delete _p;
	}
	_p = p;
}

int Context::execParse(
	const std::string & raw
)
{
	if (_p == static_cast<Parser *>(NULL)) {
		throw std::runtime_error("Parser is not set");
	}
	return _p->parse(raw);
}
