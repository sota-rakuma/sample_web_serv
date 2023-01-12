#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "ISubject.hpp"
#include "../command/ICommand.hpp"
#include <vector>
#include <list>
#include <map>
#include <poll.h>

class EventMonitor : public ISubject
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, IObserver *> _storage;
public:
	EventMonitor();
	EventMonitor(int);
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	int monitor();
	void publishEvent(int);
};

#endif /* EVENTMONITOR_HPP */
