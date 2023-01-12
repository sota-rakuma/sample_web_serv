#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "../subject/ISubject.hpp"
#include <vector>
#include <poll.h>

class EventMonitor
{
private:
	int _time;
	ISubject *_pub;
	std::vector<pollfd> _pollvec;
public:
	EventMonitor();
	EventMonitor(int);
	EventMonitor(ISubject *);
	EventMonitor(int, ISubject *);
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	int monitor();
	void publishEvent(int);
};

#endif /* EVENTMONITOR_HPP */
