#include "Utils.h"

std::vector<std::string> *splitString(const std::string &str, char delim)
{
	std::vector<std::string> *vec = new std::vector<std::string>;
	size_t first, last;

	first = 0;
	last = str.find_first_of(delim, 0);
	while (last != std::string::npos)
	{
		vec->push_back(str.substr(first, last - first));
		first = last + 1;
		last = str.find_first_of(delim, first);
	}
	vec->push_back(str.substr(first, str.length() - first + 1));
	return vec;
}
