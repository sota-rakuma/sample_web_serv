#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include "Symbol.hpp"
#include <list>
#include <vector>
#include <map>

class SyntaxTree
{
private:
	Symbol _symbol;
	std::string _raw;
	std::list<SyntaxTree> _children;
public:
	SyntaxTree();
	SyntaxTree(const Symbol &);
	SyntaxTree(const Symbol &, const std::string &);
	SyntaxTree(
		const std::pair<Symbol, std::string> &
	);
	SyntaxTree(
		const Symbol &,
		const std::list<SyntaxTree> &
	);
	SyntaxTree(
		const Symbol &,
		const std::string &,
		const std::list<SyntaxTree> &
	);
	SyntaxTree(
		const std::pair<std::string, Symbol> &,
		const std::list<SyntaxTree> &
	);
	SyntaxTree(const SyntaxTree &);
	~SyntaxTree();
	SyntaxTree &pushBackChildren(const SyntaxTree &);
	SyntaxTree &pushFrontChildren(const SyntaxTree &);
	SyntaxTree &operator=(const SyntaxTree &);
	const Symbol & getSymbol() const;
	const std::string & getRaw() const;
	const std::list<SyntaxTree> & getChildren() const;
	void getRawStrings(
		const Symbol &,
		std::list<std::string> &
	) const;
};

std::ostream &output(std::ostream &, const SyntaxTree &);
std::ostream &operator<<(std::ostream &, const SyntaxTree &);

#endif /* SYNTAXTREE_HPP */
