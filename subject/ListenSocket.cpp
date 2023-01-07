#include "ListenSocket.hpp"
#include <sys/socket.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>

#ifndef LISTEN_BACKLOG
#define LIESTEN_BACKLOG 50
#endif

ListenSocket::ListenSockError::ListenSockError()
:std::runtime_error(strerror(errno))
{
}

ListenSocket::ListenSockError::ListenSockError(
	const std::string & func_name
)
:std::runtime_error(func_name + std::string(": ") + strerror(errno))
{
}

ListenSocket::ListenSockError::~ListenSockError(){};

ListenSocket::ListenSocket(
	const std::string & ip,
	const std::string & port,
	ServerConfigFinder *conifgs
)
:ISubject(static_cast<IObserver *>(NULL)),
_ip(ip),
_port(port),
_accept(new Accept(this)),
_configs(conifgs)
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1) {
		throw ListenSockError("socket");
	}
	fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port.c_str(), &hints, &_info) != 0)
	{
		throw std::runtime_error("getaddrinfo");
	}
	if (bind(_sockfd, (const sockaddr *)_info, sizeof(addrinfo)) == -1) {
		throw std::runtime_error("bind");
	}
}

ListenSocket::ListenSocket(
	const ListenSocket & another
)
:ISubject(another.getObserver()),
_ip(another._ip),
_port(another._port),
_sockfd(another._sockfd),
_accept(new Accept(this)),
_info(another._info),
_configs(another._configs)
{
}

ListenSocket::~ListenSocket()
{
	delete _accept;
	freeaddrinfo(_info);
}

void ListenSocket::notify(
	int sockfd,
	int event,
	ISubject *subject
)
{
	getObserver()->update(sockfd, event, subject);
}

void ListenSocket::listen()
{
	if (::listen(_sockfd, LIESTEN_BACKLOG) == -1) {
		throw ListenSockError("listen");
	}
	// listen sock も new して
	notify(_sockfd, IN, this);
}

void ListenSocket::accept()
{
	socklen_t len = sizeof(addrinfo);
	int fd;
	sockaddr_in client_info;

	if (fd = ::accept(_sockfd, (sockaddr *)&client_info, &len) == -1) {
		throw ListenSockError("accept");
	}
	notify(fd, IN, new AcceptedSocket(getObserver(), fd, client_info, _configs));
}

ICommand *ListenSocket::getHandler(int event) const
{
	// 例外処理
	return _accept;
}
