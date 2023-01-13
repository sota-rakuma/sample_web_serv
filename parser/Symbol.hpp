#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <iostream>

// 非終端記号も終端記号もここで一括管理

class Symbol
{
private:
	bool _is_terminal;
	std::string _name;
public:
	Symbol();
	Symbol(bool, const std::string &);
	Symbol(const Symbol &);
	~Symbol();
	const bool isTerminal() const;
	const std::string & getName() const;
	Symbol &operator=(const Symbol&);
	bool operator==(const Symbol& rhs) const;
	bool operator!=(const Symbol& rhs) const;
	bool operator<(const Symbol & rhs) const;
	bool operator<=(const Symbol & rhs) const;
	bool operator>(const Symbol & rhs) const;
	bool operator>=(const Symbol & rhs) const;
};

std::ostream&	operator<<(std::ostream &, const Symbol&);

#include <vector>
#include <list>
typedef std::vector<Symbol> SymbolVec;
typedef std::list<Symbol> Symbollist;

#endif /* SYMBOL_HPP */
