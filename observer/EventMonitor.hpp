#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "IObserver.hpp"
#include "../command/ICommand.hpp"
#include "../handler/EventHandler.hpp"
#include <poll.h>
#include <map>
#include <vector>
#include <list>

class EventMonitor : public IObserver
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, EventHandler *> _for_find;
public:
	EventMonitor();
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	virtual void update(int, int, EventHandler *);
	void addSubject(int fd, int event, EventHandler *);
	void deleteSubject(int fd);
	int monitor(std::list<ICommand *> &);
	void postEvent(int, std::list<ICommand *> &);
};

#endif /* EVENTMONITOR_HPP */
