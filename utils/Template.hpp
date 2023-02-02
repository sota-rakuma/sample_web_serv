#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <cstddef>

template <typename T, size_t N>
size_t getSize(const T (&arr)[N])
{
	(void)arr;
	return N;
}

#endif /* TEMPLATE_HPP */
