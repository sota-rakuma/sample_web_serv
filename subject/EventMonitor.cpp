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
	ListenSocket *l =  new ListenSocket(this, &_commands, "127.0.0.1", "4242", NULL);
}

// for test
EventMonitor::EventMonitor(int time)
:_time(time)
{
	ListenSocket *l =  new ListenSocket(this, &_commands, "127.0.0.1", "4242", NULL);
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
	for (std::map<int, IObserver *>::iterator it = _storage.begin();
		it != _storage.end();
		it++)
	{
		std::map<int, IObserver *>::iterator in_it = it;
		for (;
			in_it != _storage.end();
			in_it++)
		{
			if (in_it->second == it->second) {
				break;
			}
		}
		if (in_it == _storage.end()) {
			delete it->second;
		}
		_storage.erase(it);
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
			_pollvec[i].events |= event;
			break ;
		}
	}
}

void EventMonitor::unsubscribe(
	int fd,
	bool is_dup
)
{
	try
	{
		IObserver *& target = _storage.at(fd);
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
	catch(const std::exception& e)
	{
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
			perror("poll");
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
