#ifndef ACCEPTEDSOCKET_HPP
#define ACCEPTEDSOCKET_HPP

#include "ISubject.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../handler/HTTPMethodReceiver.hpp"
#include "../command/Read.hpp"
#include "../command/Write.hpp"
#include "../config/ServerConfigFinder.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

enum Level {
	RECEIVERESPONSE,
	PARSERESPONSE,
	CREATEREQUEST,
	CHECKREQUEST,
	ROUTETARGET,
	EXECUTEMETHOD,
	//CREATERESPONSE,
};

class AcceptedSocket : public ISubject, public EventHandler
{
private:
	int _sockfd;
	sockaddr_in _info;
	HTTPRequest _req;
	HTTPResponse _res;
	Read *_read;
	Write *_write;
	HTTPMethodReceiver *_receiver;
	ServerConfigFinder *_configs;
public:
	AcceptedSocket();
	AcceptedSocket(IObserver *);
	AcceptedSocket(
		IObserver *,
		int,
		const sockaddr_in &,
		ServerConfigFinder *
	);
	AcceptedSocket(const AcceptedSocket &);
	virtual ~AcceptedSocket();
	virtual void notify(int, int, ISubject *);
	virtual ICommand *getHandler(int) const;
	AcceptedSocket &setInfo(const sockaddr_in &);
	AcceptedSocket &setFd(int);
	int processRequest(Level);
};

#endif /* ACCEPTEDSOCKET_HPP */
