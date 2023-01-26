#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "IOEventHandler.hpp"
#include "../subject/ISubject.hpp"
#include "AcceptedSocket.hpp"
#include "../command/Read.hpp"
#include "../config/ServerConfig.hpp"
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <stdexcept>
#include <map>

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
	std::string _host;
	std::string _port;
	std::map<std::string, ServerConfig> *_confs;
public:
	ListenSocket();
	ListenSocket(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		const std::string &
	);
	ListenSocket(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		const std::string &,
		std::map<std::string, ServerConfig> *
	);
	virtual ~ListenSocket();
	void setConfs(
		std::map<std::string, ServerConfig> *
	);
	void listen();
	virtual void update(int);
	virtual int read();
	virtual int write();
};

#endif /* LISTENSOCKET_HPP */
