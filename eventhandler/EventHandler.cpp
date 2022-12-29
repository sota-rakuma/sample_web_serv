#include "EventHandler.hpp"

EventHandler::EventHandler()
:_fd(0)
{
}

EventHandler::EventHandler(int fd)
:_fd(fd)
{
}

EventHandler::~EventHandler()
{
}

int EventHandler::getFd() const
{
	return _fd;
}

void EventHandler::setFd(int fd)
{
	_fd = fd;
}
