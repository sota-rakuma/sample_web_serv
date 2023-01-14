#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "IOEventHandler.hpp"
#include "../subject/ISubject.hpp"
#include "AcceptedSocket.hpp"
#include "../command/Read.hpp"
#include "../config/ServerConfigFinder.hpp"
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <stdexcept>

class ListenSocket : public IOEventHandler, public IObserver
{
private:
	ListenSocket(const ListenSocket &);
public:
	class ListenSockError : public std::runtime_error
	{
	public:
		ListenSockError();
		ListenSockError(const std::string &);
		virtual ~ListenSockError() throw();
	};
private:
	int _sockfd;
	Read * _read;
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
	virtual ~ListenSocket();
	virtual void update(int);
	virtual int read();
	virtual int write();
};

#endif /* LISTENSOCKET_HPP */
