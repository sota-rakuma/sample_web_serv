#ifndef OLDEVENTMONITOR_HPP
#define OLDEVENTMONITOR_HPP

#include "IObserver.hpp"
#include "../command/ICommand.hpp"
#include "../handler/EventHandler.hpp"
#include <poll.h>
#include <map>
#include <vector>
#include <list>

class OLDEventMonitor : public IObserver
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, EventHandler *> _for_find;
public:
	OLDEventMonitor();
	OLDEventMonitor(const OLDEventMonitor &);
	~OLDEventMonitor();
	virtual void update(int, int, EventHandler *);
	void addSubject(int fd, int event, EventHandler *);
	void deleteSubject(int fd);
	int monitor(std::list<ICommand *> &);
	void postEvent(int, std::list<ICommand *> &);
};

#endif /* OLDEVENTMONITOR_HPP */
