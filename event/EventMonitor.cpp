#include "EventMonitor.hpp"
#include <unistd.h>

EventMonitor::EventMonitor()
:_time(),
_pollvec(),
_clnts(),
_files()
{
}

EventMonitor::EventMonitor(
	const EventMonitor & another
)
:_time(another._time),
_pollvec(another._pollvec)
{
	for (std::map<int, Client *>::const_iterator it = another._clnts.begin();
		it != another._clnts.end();
		it++)
	{
		_clnts[it->first] = it->second;
	}

	for (std::map<int, File *>::const_iterator it = another._files.begin();
		it != another._files.end();
		it++)
	{
		_files[it->first] = it->second;
	}
}

EventMonitor::~EventMonitor()
{
	for (std::map<int, Client *>::const_iterator it = _clnts.begin();
		it != _clnts.end();
		it++)
	{
		delete it->second;
	}

	for (std::map<int, File *>::const_iterator it = _files.begin();
		it != _files.end();
		it++)
	{
		delete it->second;
	}
}

void EventMonitor::setEvent(int fd, int event)
{
	for (size_t i = 0; i < _pollvec.size(); i++) {
		if (_pollvec[i].fd == fd) {
			_pollvec[i].events |= event;
			break;
		}
	}
}

void EventMonitor::removeEvent(int fd, int event)
{
	for (size_t i = 0; i < _pollvec.size(); i++) {
		if (_pollvec[i].fd == fd) {
			_pollvec[i].events ^= event;
			break;
		}
	}
}

void EventMonitor::addClient(
	int sockfd,
	const sockaddr_in & addr
)
{
	_pollvec.push_back((pollfd){sockfd, POLLIN, 0});
	_clnts.insert(std::make_pair(sockfd, new Client(sockfd, addr)));
}

void EventMonitor::addFile(
	int sockfd,
	int event
)
{
	_pollvec.push_back((pollfd){sockfd, event, 0});
}

void EventMonitor::deleteClient(int fd)
{
	_clnts.erase(fd);
	for (size_t i = 0; _pollvec.size(); i++)
	{
		if (_pollvec[i].fd == fd) {
			_pollvec.erase(_pollvec.begin() + i);
		}
	}
}

void EventMonitor::deleteFile(int fd)
{
	_files.erase(fd);
	for (size_t i = 0; _pollvec.size(); i++)
	{
		if (_pollvec[i].fd == fd) {
			_pollvec.erase(_pollvec.begin() + i);
		}
	}
}

State EventMonitor::monitor()
{
	int ready;

	ready = poll(_pollvec.data(), _pollvec.size(), _time);
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
	postEvent(ready);
	return SUCCESS;
}

// ここではハンドラーの登録をするんだけど、一々newしてたら性能が落ちるから、CLIENT側とかで既に登録されてるものを単に代入するにとどめたい
// そうなるとレシーバは使えないか？
void EventMonitor::postEvent(int ready)
{
	for (size_t i = 0; i < _pollvec.size() && ready > 0; i++)
	{

	}
}

void EventMonitor::handleEvent()
{
	while (_events.size() > 0)
	{
		EventHandler *& ref = *_events.begin();
		// 各コマンドで例外処理は必要かもしれない
		ref->execute();
		_events.pop_front();
	}
}