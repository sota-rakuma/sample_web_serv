#ifndef ACCEPTEDSOCKET_HPP
#define ACCEPTEDSOCKET_HPP

#include "EventHandler.hpp"
#include "ISubject.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "CGI.hpp"
#include "File.hpp"
#include "../command/Get.hpp"
#include "../command/Post.hpp"
#include "../command/Delete.hpp"
#include "../config/ServerConfigFinder.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class AcceptedSocket : public ISubject, public EventHandler
{
private:
	int _sockfd;
	sockaddr_in _info;
	HTTPRequest _req;
	HTTPResponse _res;
	File *_file;
	CGI *_cgi;
	Get *_get;
	Post *_post;
	Delete *_delete;
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
};

#endif /* ACCEPTEDSOCKET_HPP */
