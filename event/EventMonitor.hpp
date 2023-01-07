#ifndef EVENTMONITOR_HPP
#define EVENTMONITOR_HPP

#include "Client.hpp"
#include "File.hpp"
#include "Write.hpp"
#include "Read.hpp"
#include <vector>
#include <map>
#include <list>
#include <poll.h>

enum State {
	SUCCESS,
	ERROR,
	CONTINUE,
};

class EventMonitor
{
private:
	int _time;
	std::vector<pollfd> _pollvec;
	std::map<int, Client *> _clnts;
	std::map<int, File *> _files;
	std::list<EventHandler *> _events;

	//std::vector<Client *> _clnts;
	//std::vector<File *> _files;
public:
	EventMonitor();
	EventMonitor(const EventMonitor &);
	~EventMonitor();
	void setEvent(int, int);
	void removeEvent(int, int);
	void addClient(
		int,
		const sockaddr_in &
	);
	void addFile(int, int);
	void deleteClient(int);
	void deleteFile(int);
	State monitor();
	void postEvent(int);
	void handleEvent();
};

#endif /* EVENTMONITOR_HPP */
