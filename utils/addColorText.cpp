#include <sstream>
#include "utils.hpp"

const std::string addColorText(const std::string & raw, enum color kind)
{
	std::stringstream ss;
	ss << "\033[" << static_cast<int>(kind) << "m" << raw << "\033[39m";
	return ss.str();
}

const std::string addColorText(int raw, enum color kind)
{
	std::stringstream ss;
	ss << "\033[" << static_cast<int>(kind) << "m" << raw << "\033[39m";
	return ss.str();
}
