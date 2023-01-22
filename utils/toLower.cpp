#include "utils.hpp"

char ft_tolower(char c)
{
	if ('A' <= c && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}