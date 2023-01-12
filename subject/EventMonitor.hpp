#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP


/*

confのパース → listen socketの生成 → イベントループ

class Server
{
private:
	EventMonitor *_em;
	std::list<ICommand *> _commands;
public:
	void run() {
		while (true) {
			em->monitor(_commands);
			triggerEvent();
		}
	};
}

*/
#include "ISubject.hpp"
#include <vector>
#include <map>
#include <poll.h>

class EventMonitor : public ISubject
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, IObserver *> _storage;
private:
	void publishEvent(
		int,
		std::list<ICommand *> &
	);
public:
	EventMonitor();
	EventMonitor(int);
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	int monitor(std::list<ICommand *> &);
};

#endif /* EVENTMONITOR_HPP */
