#include "SyntaxTree.hpp"
#include "utils/utils.hpp"

SyntaxTree::SyntaxTree()
{
}

SyntaxTree::SyntaxTree(const Symbol &token)
:_symbol(token)
{
}

SyntaxTree::SyntaxTree(
	const std::pair<Symbol, std::string> & p
)
:_symbol(p.first),
_raw(p.second)
{
}

SyntaxTree::SyntaxTree(
	const Symbol &token,
	const std::string &raw
)
:_symbol(token),
_raw(raw)
{
}

SyntaxTree::SyntaxTree(
	const Symbol &token,
	const std::list<SyntaxTree> &children
)
:_symbol(token),
_children(children)
{
}

SyntaxTree::SyntaxTree(
	const Symbol &token,
	const std::string &raw,
	const std::list<SyntaxTree> &children
)
:_symbol(token),
_raw(raw),
_children(children)
{
}

SyntaxTree::SyntaxTree(
	const std::pair<std::string, Symbol> & p,
	const std::list<SyntaxTree> & children
)
:_symbol(p.second),
_raw(p.first),
_children(children)
{
}

SyntaxTree::SyntaxTree(const SyntaxTree &another)
:_symbol(another._symbol),
_raw(another._raw),
_children(another._children)
{
}

SyntaxTree::~SyntaxTree()
{
}

SyntaxTree &SyntaxTree::pushBackChildren(const SyntaxTree &child)
{
	_children.push_back(child);
	return *this;
}

SyntaxTree &SyntaxTree::pushFrontChildren(const SyntaxTree &child)
{
	_children.push_front(child);
	return *this;
}

SyntaxTree &SyntaxTree::operator=(const SyntaxTree &rhs)
{
	_symbol = rhs._symbol;
	_raw = rhs._raw;
	_children = rhs._children;
	return *this;
}

const Symbol & SyntaxTree::getSymbol() const
{
	return _symbol;
}

const std::string & SyntaxTree::getRaw() const
{
	return _raw;
}

const std::list<SyntaxTree> & SyntaxTree::getChildren() const
{
	return _children;
}

static void setRawString(
	std::string &raw,
	const SyntaxTree & target
)
{
	if (target.getSymbol().isTerminal() == true) {
		raw += target.getRaw();
		return ;
	}
	for (std::list<SyntaxTree>::const_iterator it = target.getChildren().begin();
		it != target.getChildren().end();
		it++)
	{
		setRawString(raw, *it);
	}
}

void SyntaxTree::getRawStrings(
	const Symbol & target,
	std::list<std::string> &candidate
) const
{
	for (std::list<SyntaxTree>::const_iterator it = _children.begin();
		it != _children.end();
		it++)
	{
		if (_symbol == target) {
			std::string raw;
			setRawString(raw, *it);
			candidate.push_back(raw);
		} else {
			it->getRawStrings(target, candidate);
		}
	}
}

std::ostream &output(std::ostream &os, const SyntaxTree &st)
{
	os << "Token: " << st.getSymbol() << std::endl
	<< "Raw: " << st.getRaw() << std::endl
	<< "Children: " << std::endl
	<< "{";

	for (std::list<SyntaxTree>::const_iterator it = st.getChildren().begin();
		it != st.getChildren().end();
		it++)
	{
		if (it != st.getChildren().begin()) {
			os << ", ";
		}
		os << "<Token: " << it->getSymbol() << ", "
		<< "Raw: " << it->getRaw() << ">";
	}
	os << "}" << std::endl;
	os << std::endl;
	for (std::list<SyntaxTree>::const_iterator it = st.getChildren().begin();
		it != st.getChildren().end();
		it++)
	{
		output(os, *it);
	}
	return os;
}

std::ostream &operator<<(std::ostream & os, const SyntaxTree & st)
{
	os << addColorText("🌲🌲🌲🌲🌲🌲🌲🌲SyntaxTree🌲🌲🌲🌲🌲🌲🌲🌲", GREEN) << std::endl;
	output(os, st);
	os << "🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲🌲" << std::endl;
	return os;
}