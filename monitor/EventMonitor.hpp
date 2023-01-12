#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "../publisher/IPublisher.hpp"
#include <vector>
#include <poll.h>

class EventMonitor
{
private:
	int _time;
	ISubscriber *_a;
	IPublisher *_pub;
	std::vector<pollfd> _pollvec;
public:
	EventMonitor();
	EventMonitor(int);
	EventMonitor(IPublisher *);
	EventMonitor(int, IPublisher *);
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	int monitor();
};

#endif /* EVENTMONITOR_HPP */
