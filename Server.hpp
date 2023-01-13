#ifndef Server_HPP
#define Server_HPP

#include "subject/EventMonitor.hpp"
#include "ServerConfigFinder.hpp"

class Server
{
private:
	EventMonitor _ev;
	// config パーサー
public:
	Server();
	Server(int);
	Server(const Server &);
	~Server();
	void init();
	void run();
};


#endif /* Server_HPP */
