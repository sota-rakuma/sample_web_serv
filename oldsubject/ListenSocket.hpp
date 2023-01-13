#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "../handler/EventHandler.hpp"
#include "AcceptedSocket.hpp"
#include "../command/Accept.hpp"
#include "../config/ServerConfigFinder.hpp"
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <stdexcept>

class ListenSocket : public EventHandler
{
private:
	ListenSocket::ListenSocket(const ListenSocket &);
public:
	class ListenSockError : public std::runtime_error
	{
	public:
		ListenSockError();
		ListenSockError(const std::string &);
		~ListenSockError();
	};
private:
	int _sockfd;
	Accept * _accept;
	std::string _ip;
	std::string _port;
	ServerConfigFinder *_configs;
public:
	ListenSocket(
		const std::string &,
		const std::string &,
		ServerConfigFinder *
	);
	~ListenSocket();
	virtual void notify(int, int, EventHandler *);
	virtual ICommand *getHandler(int) const;
	void listen();
	void accept();
};

#endif /* LISTENSOCKET_HPP */
