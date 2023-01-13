#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "IOEventHandler.hpp"
#include "../subject/ISubject.hpp"
#include "AcceptedSocket.hpp"
#include "../command/Accept.hpp"
#include "../config/ServerConfigFinder.hpp"
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <stdexcept>

class ListenSocket : public IOEventHandler, public IObserver
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
	ListenSocket();
	ListenSocket(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		const std::string &,
		ServerConfigFinder *
	);
	~ListenSocket();
	virtual void update(int);
	virtual int read();
	virtual int write();
	virtual ICommand *getHandler(int) const;
};

#endif /* LISTENSOCKET_HPP */
