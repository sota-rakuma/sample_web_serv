#include "EventMonitor.hpp"
#include <cstddef>
#include <unistd.h>

EventMonitor::EventMonitor()
:_time(300)
{
}

EventMonitor::EventMonitor(int time)
:_time(time)
{
}

EventMonitor::EventMonitor(const EventMonitor &another)
:_time(another._time),
_pollvec(another._pollvec),
_storage(another._storage)
{
}

EventMonitor::~EventMonitor()
{
}


int EventMonitor::monitor(
	std::list<ICommand *> & commands
)
{
	int ready;

	ready = poll(_pollvec.data(), _pollvec.size(), _time);
	if (ready <= 0) {
		if (ready == 0 || errno != EINTR) {
			// タイムアウトはここで実装できる
			usleep(500);
			return 0;
		}
		return -1;
	}
	publishEvent(ready, commands);
	return 1;
}

void EventMonitor::publishEvent(
	int ready,
	std::list<ICommand *> & commands
)
{
	for (size_t i = 0; i < _pollvec.size() && ready > 0; i++)
	{
		if (_pollvec[i].revents == 0) {
			continue;
		}
		int fd = _pollvec[i].fd;
		notify(_pollvec[i].fd, commands);
		ready -= 1;
	}
}
