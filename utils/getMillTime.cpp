#include <sys/time.h>

long get_mtime(void)
{
	struct timeval tv;
	long num;

	gettimeofday(&tv, NULL);
	num = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return num;
}
