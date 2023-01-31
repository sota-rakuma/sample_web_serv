#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "ISubject.hpp"
#include "../handler/ListenSocket.hpp"
#include <vector>
#include <map>
#include <poll.h>

class EventMonitor : public ISubject
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, IObserver *> _storage;
	std::map<long, int> _time_manager;
private:
	void publishEvent(int);
	void notifyTimeOut();
	int findTimer();
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
