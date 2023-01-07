#ifndef Server_HPP
#define Server_HPP

#include <vector>
#include <list>
#include <poll.h>
#include <stdexcept>
#include "ServerConfigFinder.hpp"


#define READ 1
#define WRITE 2
#define ACCEPT 4

class Server
{
public:
	typedef std::pair<int, int> Event;
	class PollError : public std::runtime_error
	{
	public:
		PollError();
		~PollError();
	};
	enum State {
		SUCCESS,
		ERROR,
		CONTINUE,
	};
private:
	std::vector<pollfd> _pollfd;
	std::list<Server::Event> _events;
	size_t _lssize;
	int _timeout;
	State monitorEvent();
	void traversePollfd(int);
	void handleError(
		size_t
	);
	void handleEvents();
	int handleOutput(
		size_t
	);
	int handleInput(
		size_t
	);
	void deleteConnection(
		size_t
	);
public:
	Server();
	Server(int);
	Server(const Server &);
	~Server();
	void run();
	const std::vector<pollfd> &getPollfd() const;
	const std::list<Event> & getEvents() const;
};


#endif /* Server_HPP */
