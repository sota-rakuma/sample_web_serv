#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "ISubject.hpp"
#include "../handler/ListenSocket.hpp"
#include <vector>
#include <map>
#include <poll.h>
#include <list>

class EventMonitor : public ISubject
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, IObserver *> _storage;
	std::list<std::pair<long, int> > _tm;
private:
	void publishEvent(int);
	void notifyTimeOut();
	int findTimer();
	void insertTimer(
		long,
		int
	);
public:
	EventMonitor();
	EventMonitor(int);
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	virtual void notify(int, int);
	virtual void subscribe(int, int, IObserver *, long);
	virtual void unsubscribe(int, bool);
	int monitor();
};

#endif /* EVENTMONITOR_HPP */
