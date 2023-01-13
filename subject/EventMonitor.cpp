#include "EventMonitor.hpp"
#include <cstddef>
#include <unistd.h>
#include <typeinfo>
#include <string>

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
_storage(another._storage),
_commands(another._commands)
{
}

EventMonitor::~EventMonitor()
{
}

void EventMonitor::notify(int fd, int event)
{
	_storage[fd]->update(event);
}

void EventMonitor::subscribe(
	int fd,
	int event,
	IObserver * target
)
{
	_storage[fd] = target;
	_pollvec.push_back((pollfd){fd, event, 0});
}

void EventMonitor::unsubscribe(
	int fd,
	bool is_dup,
	IObserver *target
)
{
	if (is_dup == false) {
		delete _storage[fd];
	}
	_storage.erase(fd);
	for (size_t i = 0; i < _pollvec.size(); i++) {
		if (fd == _pollvec[i].fd) {
			_pollvec.erase(_pollvec.begin() + i);
			break;
		}
	}
}

void EventMonitor::monitor()
{
	while (true) {
		int ready;

		ready = poll(_pollvec.data(), _pollvec.size(), _time);
		if (ready <= 0) {
			if (ready == 0 || errno != EINTR) {
				// タイムアウトはここで実装できる
				usleep(500);
				continue;
			}
			break;
		}
		publishEvent(ready);
		triggerEvent();
	}
}

void EventMonitor::publishEvent(int ready)
{
	for (size_t i = 0; i < _pollvec.size() && ready > 0; i++)
	{
		if (_pollvec[i].revents == 0) {
			continue;
		}
		notify(_pollvec[i].fd, _pollvec[i].revents);
		ready -= 1;
	}
}

void EventMonitor::triggerEvent()
{
	while (_commands.size() > 0) {
		(*(_commands.begin()))->execute();
		const std::string &name = typeid(*_commands.begin()).name();
		if (name == "Get" || name == "Post" || name == "Delete") {
			delete *_commands.begin();
		}
		_commands.pop_front();
	}
}
