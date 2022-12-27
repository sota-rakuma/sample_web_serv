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

#endif /* UTILS_HPP */
