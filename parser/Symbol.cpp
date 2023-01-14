#include "Symbol.hpp"
#include "../utils/utils.hpp"

Symbol::Symbol()
{
}

Symbol::Symbol(bool is_terminal, const std::string &name)
: _is_terminal(is_terminal),
_name(name)
{
}

Symbol::Symbol(const Symbol &another)
: _is_terminal(another._is_terminal),
_name(another._name)
{
}

Symbol::~Symbol()
{
}

const bool Symbol::isTerminal() const
{
	return _is_terminal;
}

const std::string & Symbol::getName() const
{
	return _name;
}

Symbol &Symbol::operator=(const Symbol& rhs)
{
	_is_terminal = rhs._is_terminal;
	_name = rhs._name;
	return *this;
}

bool Symbol::operator==(const Symbol& rhs) const
{
	return _is_terminal == rhs._is_terminal &&
			_name == rhs._name;
}

bool Symbol::operator!=(const Symbol& rhs) const
{
	return !(*this == rhs);
}

bool Symbol::operator<(const Symbol & rhs) const
{
	return (this->_name < rhs._name) && ~(this->_is_terminal < rhs._is_terminal);
}

bool Symbol::operator<=(const Symbol & rhs) const
{
	return !(*this > rhs);
}

bool Symbol::operator>(const Symbol & rhs) const
{
	return this->_name > rhs._name && ~(this->_is_terminal > rhs._is_terminal);
}

bool Symbol::operator>=(const Symbol & rhs) const
{
	return !(*this < rhs);
}

std::ostream&	operator<<(std::ostream &os, const Symbol &symbol)
{
	if (symbol.isTerminal()) {
		os << addColorText("TERMINAL", GREEN);
	} else {
		os << addColorText("NONTERMINAL", BLUE);
	}
	os << ": " <<  symbol.getName();
	return os;
}
