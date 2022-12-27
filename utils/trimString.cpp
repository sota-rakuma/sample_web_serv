#include "Utils.h"

void trimString(std::string& str, const std::string &set)
{
	size_t first = str.find_first_not_of(set);
	if (first == std::string::npos) {
		return ;
	}
	size_t last = str.find_last_not_of(set);
	if (last == std::string::npos) {
		return ;
	}
	if (first == 0 && last == str.length() - 1) {
		return ;
	}
	str = str.substr(first, last - first + 1);
}