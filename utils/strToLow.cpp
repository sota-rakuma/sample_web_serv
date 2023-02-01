#include "utils.hpp"

void strToLow(
	std::string & target,
	const std::string & raw
)
{
	target = raw;
	for (size_t i = 0; i < target.size(); i++)
	{
		if (isUpperCase(target[i])) {
			target[i] = toLower(target[i]);
		}
	}
}