#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

bool ftIsLowerCase(char ch);
bool ftIsUpperCase(char ch);
bool ftIsAlpha(char ch);
bool ftIsNumber(char ch);
bool ftIsHexDigit(char ch);
int ft_stoi(const std::string& str, std::size_t* idx, int base);
std::vector<std::string> *splitString(const std::string &str, char delim);
void trimString(std::string& str, const std::string &set);

#endif /* UTILS_H */
