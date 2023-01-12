#include "EventMonitor.hpp"
#include <cstddef>
#include <unistd.h>

EventMonitor::EventMonitor()
:_time(300),
_pub(static_cast<IPublisher *>(NULL))
{
}

EventMonitor::EventMonitor(int time)
:_time(time)
{
}

EventMonitor::EventMonitor(IPublisher *pub)
:_pub(pub)
{
}

EventMonitor::EventMonitor(int time, IPublisher * pub)
:_time(time),
_pub(pub)
{
}


EventMonitor::EventMonitor(const EventMonitor &another)
:_time(another._time),
_pub(another._pub),
_pollvec(another._pollvec)
{
}

EventMonitor::~EventMonitor()
{
}


int EventMonitor::monitor()
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
}

void EventMonitor::publishEvent(int ready)
{
	for (size_t i = 0; i < _pollvec.size() && ready > 0; i++)
	{
		if (_pollvec[i].revents == 0) {
			continue;
		}
		int fd = _pollvec[i].fd;
		_pub->notify(_pollvec[i].fd);
		ready -= 1;
	}
}
