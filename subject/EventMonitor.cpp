#include "EventMonitor.hpp"
#include <cstddef>
#include <unistd.h>
#include <typeinfo>
#include <string>

// for test
EventMonitor::EventMonitor()
:_time(300)
{
	if (setenv("GATEWAY_INTERFACE", "CGI/1.1", 1) == -1 ||
		setenv("SERVER_PROTOCOL", "HTTP/1.1", 1) == -1 ||
		setenv("SERVER_SOFTWARE", "42WebServ", 1) == -1)
	{
		perror("setenv");
		exit(1);
	}
}

// for test
EventMonitor::EventMonitor(int time)
:_time(time)
{
}

EventMonitor::EventMonitor(const EventMonitor &another)
:_time(another._time),
_pollvec(another._pollvec),
_storage(another._storage)
//_commands(another._commands)
{
}

EventMonitor::~EventMonitor()
{
	while (_storage.size() > 0) {
		std::map<int, IObserver *>::iterator first = _storage.begin();
		std::map<int, IObserver *>::iterator in_it = first;
		in_it++;
		for (; in_it != _storage.end(); in_it++)
		{
			if (in_it->second == first->second) {
				break;
			}
		}
		if (in_it == _storage.end()) {
			delete first->second;
		}
		_storage.erase(first);
	}
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
	if (_storage.find(fd) == _storage.end()) {
		_storage[fd] = target;
		_pollvec.push_back((pollfd){fd, event, 0});
		return ;
	}

	for (size_t i = 0; i < _pollvec.size(); i++) {
		if (_pollvec[i].fd == fd) {
			_pollvec[i].events = event;
			break ;
		}
	}
}

void EventMonitor::unsubscribe(
	int fd,
	bool is_dup
)
{
	std::map<int, IObserver *>::iterator it = _storage.find(fd);
	if (it == _storage.end()) {
		return ;
	}
	if (is_dup == false) {
		delete it->second;
	}
	_storage.erase(fd);
	for (size_t i = 0; i < _pollvec.size(); i++) {
		if (fd == _pollvec[i].fd) {
			_pollvec.erase(_pollvec.begin() + i);
			break;
		}
	}
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
		perror("poll");
		return -1;
	}
	publishEvent(ready);
	return 0;
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
