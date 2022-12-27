#include "Server.hpp"
#include <unistd.h>
#include <iostream>

Server::PollError::PollError()
: std::runtime_error(
	std::string("poll: ") + std::string(strerror(errno)))
{
}

Server::PollError::~PollError()
{
}

Server::Server()
:_pollfd(),
_events(),
_timeout()
{
}

Server::Server(int timeout)
:_pollfd(),
_events(),
_timeout(timeout)
{
}

Server::Server(const Server & another)
:_pollfd(another._pollfd),
_events(another._events),
_timeout(another._timeout)
{
}

Server::~Server()
{
}

void Server::run()
{
	_lssize = _pollfd.size();
	for (;;)
	{
		int state = monitorEvent();
		if (state == CONTINUE) {
			continue;
		} else if (state == ERROR){
			perror("poll");
			break ;
		}
		handleEvents();
	}
}

/**
 * @brief
 *
 * @return State
 */
Server::State
Server::monitorEvent()
{
	int ready;

	ready = poll(_pollfd.data(), _pollfd.size(), _timeout);
	if (ready < 0) {
		if (errno != EINTR) {
			return CONTINUE;
		}
		return ERROR;
	} else if (ready == 0) {
		std::cout << "waiting event" << std::endl;
		sleep(1);
		return CONTINUE;
	}
	traversePollfd(ready);
	return SUCCESS;
}

void Server::traversePollfd(int ready)
{
	for (size_t i = 0; i < _pollfd.size() && ready > 0; i++)
	{
		if (_pollfd[i].revents == 0) {
			continue ;
		}
		if (_pollfd[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
			handleError(i);
		} else {
			int state;
			if (_pollfd[i].revents & POLLOUT) {
				state = WRITE;
			} else if (i < _lssize) {
				state = ACCEPT;
			} else {
				state = READ;
			}
			_events.push_back(std::make_pair(
				_pollfd[i].fd,
				state
			));
		}
		ready--;
	}
}

void Server::handleError(size_t index)
{
	if (_pollfd[index].revents & POLLERR) {
		std::cerr << "Exception happened on device or socket" << std::endl;
	} else if (_pollfd[index].revents & POLLHUP) {
		std::cerr << "Client disconnected" << std::endl;
	} else if (_pollfd[index].revents & POLLNVAL) {
		std::cerr << "fd is not opend" << std::endl;
	}
	deleteConnection(index);
}

void Server::handleEvents()
{
	for (std::list<Event>::iterator it = _events.begin();
		it != _events.end();
		it++)
	{
		if (it->second == ACCEPT) {
		} else if (it->second == READ) {
		} else {
		}
	}
}

int Server::handleOutput(size_t index)
{
}

int Server::handleInput(size_t index)
{
}

void Server::deleteConnection(size_t index)
{
	_pollfd.erase(_pollfd.begin() + index);
}

const std::vector<pollfd> &Server::getPollfd() const
{
	return _pollfd;
}

const std::list<Server::Event> & Server::getEvents() const
{
	return _events;
}
