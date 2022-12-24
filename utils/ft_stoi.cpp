#include "Utils.h"
#include <cerrno>
#include <stdexcept>

int ft_stoi(const std::string& str, std::size_t* idx, int base)
{
	const char* p = str.c_str();
	char* end;
	errno = 0;
	long x = std::strtol(p, &end, base);
	if (p == end) {
		throw std::invalid_argument("ft_stoi");
	}
	if (errno == ERANGE || x < INT_MIN || x > INT_MAX) {
		throw std::out_of_range("ft_stoi");
	}
	if (idx != NULL) {
		*idx = static_cast<std::size_t>(end - p);
	}
	return static_cast<int>(x);
}