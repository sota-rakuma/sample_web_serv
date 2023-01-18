#include "utils.hpp"

bool isHexDig(char c)
{
	return isDigit(c) ||
		(('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'));
}