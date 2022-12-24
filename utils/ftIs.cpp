#include "Utils.h"
#include <string>

bool ftIsLowerCase(char ch)
{
	return 'a' <= ch && ch <= 'z';
}

bool ftIsUpperCase(char ch)
{
	return 'A' <= ch && ch <= 'Z';
}

bool ftIsAlpha(char ch)
{
	return ftIsLowerCase(ch) || ftIsUpperCase(ch);
}

bool ftIsNumber(char ch)
{
	return std::string("0123456789").find_first_of(ch) != std::string::npos;
}

bool ftIsHexDigit(char ch)
{
	return std::string("0123456789ABCDEFabcdef").find_first_of(ch) != std::string::npos;
}