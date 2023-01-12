#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP


/*

confのパース → listen socketの生成 → イベントループ

class Server
{
private:
	EventMonitor *_em;
public:
	void run() {
		em->monitor();
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
	std::list<ICommand *> _commands;
private:
	void publishEvent(int);
public:
	EventMonitor();
	EventMonitor(int);
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	virtual void notify(int, int);
	virtual void subscribe(int, int, IObserver *);
	virtual void unsubscribe(int, bool, IObserver *);
	void monitor();
	void triggerEvent();
};

#endif /* EVENTMONITOR_HPP */
