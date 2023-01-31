#include <sys/time.h>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <cerrno>

long getMilliTime()
{
	struct timeval tv;
	long num;

	gettimeofday(&tv, NULL);
	num = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (num == -1) {
		throw std::runtime_error(strerror(errno));
	}
	return num;
}
