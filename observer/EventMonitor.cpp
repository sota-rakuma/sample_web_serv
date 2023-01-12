#include "EventMonitor.hpp"
#include <unistd.h>

EventMonitor::EventMonitor()
:_pollvec()
{
}

EventMonitor::EventMonitor(const EventMonitor & another)
:_pollvec(another._pollvec)
{
}

EventMonitor::~EventMonitor()
{
}

void EventMonitor::update(int fd, int event, EventHandler * subject)
{
	if (event == REMOVE) {
		for (size_t i = 0; i < _pollvec.size(); i++) {
			if (_pollvec[i].fd == fd) {
				_pollvec.erase(_pollvec.begin() + i);
			}
		}
		delete _for_find[fd];
		_for_find.erase(fd);
	} else {
		addSubject(fd, event, subject);
	}
}

void EventMonitor::addSubject(int fd, int event, EventHandler * subject)
{
	if (event & OUT) {
		_pollvec.push_back((pollfd){fd, POLLOUT, 0});
	} else if (event & IN) {
		_pollvec.push_back((pollfd){fd, POLLIN, 0});
	}
	_for_find.insert(std::make_pair(fd, subject));
}

void EventMonitor::deleteSubject(int fd)
{
	delete _for_find[fd];
	_for_find.erase(fd);
}

int EventMonitor::monitor(
	std::list<ICommand *> & commandlist
)
{
	int ready;

	ready = poll(_pollvec.data(), _pollvec.size(), _time);
	if (ready <= 0) {
		if (ready == 0 || errno != EINTR) {
			// タイムアウトはここで実装できるかも
			usleep(500);
			return 0;
		}
		return -1;
	}
	postEvent(ready, commandlist);
	return 1;
}

void EventMonitor::postEvent(
	int ready,
	std::list<ICommand *> & commandlist
)
{
	for (size_t i = 0; i < _pollvec.size() && ready > 0; i++)
	{
		if (_pollvec[i].revents == 0) {
			continue;
		}
		int fd = _pollvec[i].fd;
		commandlist.push_back(_for_find[fd]->getHandler(_pollvec[i].revents));
		ready -= 1;
		//if (_pollvec[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
		//}
	}
}