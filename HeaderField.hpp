#ifndef HEADERFIELD_HPP
#define HEADERFIELD_HPP

#include <string>
#include <iostream>

class HeaderField
{
private:
	std::string _name;
	std::string _val;
public:
	HeaderField();
	HeaderField(const std::string &);
	HeaderField(
		const std::string &,
		const std::string &
	);
	HeaderField(const HeaderField &);
	~HeaderField();
	const std::string & getName() const;
	const std::string & getValue() const;
	HeaderField & setName(const std::string &);
	HeaderField & setValue(const std::string &);
};

std::ostream &operator<<(std::ostream &, const HeaderField &);

#endif /* HEADERFIELD_HPP */
