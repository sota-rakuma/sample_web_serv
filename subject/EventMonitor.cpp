#include "EventMonitor.hpp"
#include <cstddef>
#include <unistd.h>
#include <typeinfo>
#include <string>

EventMonitor::EventMonitor()
:_time(300),
_pollvec(),
_storage()
{
	if (setenv("GATEWAY_INTERFACE", "CGI/1.1", 1) == -1 ||
		setenv("SERVER_PROTOCOL", "HTTP/1.1", 1) == -1 ||
		setenv("SERVER_SOFTWARE", "42WebServ", 1) == -1)
	{
		perror("setenv");
		exit(1);
	}
}

EventMonitor::EventMonitor(int time)
:_time(time),
_pollvec(),
_storage()
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

void EventMonitor::insertTimer(
	long timeout,
	int fd
)
{
	if (_tm.size() == 0) {
		_tm.push_back(std::make_pair(timeout, fd));
		return ;
	}
	for (std::list<std::pair<long, int> >::iterator it = _tm.begin();
		it != _tm.end();
		it++)
	{
		if (it->first <= timeout) {
			_tm.insert(it, std::make_pair(timeout, fd));
			break;
		}
	}
}


void EventMonitor::subscribe(
	int fd,
	int event,
	IObserver * target,
	long timeout
)
{
	if (_storage.find(fd) == _storage.end()) {
		_storage[fd] = target;
		_pollvec.push_back((pollfd){fd, event, 0});
		if (timeout > 0) {
			insertTimer(timeout, fd);
		}
		return ;
	}

	for (size_t i = 0; i < _pollvec.size(); i++) {
		if (_pollvec[i].fd == fd) {
			_pollvec[i].events = event;
			break ;
		}
	}

	for (std::list<std::pair<long, int> >::iterator it = _tm.begin();
		it != _tm.end();
		it++)
	{
		if (fd == it->second) {
			_tm.erase(it);
			break;
		}
	}

	if (timeout > 0) {
		insertTimer(timeout, fd);
	}
}

void EventMonitor::unsubscribe(
	int fd,
	bool wanna_keep
)
{
	std::map<int, IObserver *>::iterator it = _storage.find(fd);
	if (it == _storage.end()) {
		return ;
	}
	for (size_t i = 0; i < _pollvec.size(); i++) {
		if (fd == _pollvec[i].fd) {
			_pollvec.erase(_pollvec.begin() + i);
			break;
		}
	}
	if (wanna_keep == false) {
		delete it->second;
	}
	_storage.erase(fd);

	for (std::list<std::pair<long, int> >::iterator it = _tm.begin();
		it != _tm.end();
		it++)
	{
		if (fd == it->second) {
			_tm.erase(it);
			break;
		}
	}
}

int EventMonitor::monitor()
{
	int ready;

	ready = poll(_pollvec.data(), _pollvec.size(), findTimer());
	if (ready <= 0) {
		if (ready == 0) {
			notifyTimeOut();
			return 0;
		}
		perror("poll");
		return -1;
	}
	publishEvent(ready);
	return 0;
}

int EventMonitor::findTimer()
{
	long timer = 0;

	if (_tm.size() == 0) {
		return -1;
	}
	while (_tm.begin() != _tm.end())
	{
		timer = _tm.begin()->first - getMilliTime();
		if (timer <= 0) {
			notify(_tm.begin()->second, EV_TIMEOUT);
		} else {
			break;
		}
	}
	if (_tm.begin() == _tm.end()) {
		return -1;
	}

	if (timer > INT_MAX) {
		return -1;
	}
	return (static_cast<int>(timer));
}

void EventMonitor::notifyTimeOut()
{
	long now = getMilliTime();

	for (std::list<std::pair<long, int> >::iterator it =_tm.begin();
		it != _tm.end();)
	{
		if (it->first <= now) {
			notify((it++)->second, EV_TIMEOUT);
		} else {
			++it;
		}
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
