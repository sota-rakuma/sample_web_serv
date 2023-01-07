#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "IObserver.hpp"
#include "../command/ICommand.hpp"
#include <poll.h>
#include <map>
#include <vector>
#include <list>

class EventMonitor : public IObserver
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, ISubject *> _for_find;
public:
	EventMonitor();
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	virtual void update(int, int, ISubject *);
	void addSubject(int fd, int event, ISubject *);
	void deleteSubject(int fd);
	int monitor(std::list<ICommand *> &);
	void postEvent(int, std::list<ICommand *> &);
};

#endif /* EVENTMONITOR_HPP */
