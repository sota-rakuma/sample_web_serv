#include "utils.hpp"

char toLower(char c)
{
	if ('A' <= c && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}