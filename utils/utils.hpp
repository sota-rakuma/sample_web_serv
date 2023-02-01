#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

enum color {
	BlACK = 30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
};

const std::string addColorText(const std::string &, enum color);
const std::string addColorText(int raw, enum color kind);
bool isDigit(char);
bool isAlpha(char);
bool isUpperCase(char);
bool isLowerCase(char);
bool isHexDig(char);
bool isPrintable(char);
char toLower(char);
long getMilliTime();
void strToLow(
	std::string & target,
	const std::string & raw
);

#endif /* UTILS_HPP */
