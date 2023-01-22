#include "ListenSocket.hpp"
#include <sys/socket.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

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

ListenSocket::ListenSockError::~ListenSockError() throw()
{};

ListenSocket::ListenSocket()
:IOEventHandler()
{
}

ListenSocket::ListenSocket(
	ISubject * subject,
	std::list<ICommand *> *commands,
	const std::string & ip,
	const std::string & port,
	ServerConfigFinder *conifgs
)
:IOEventHandler(subject, commands),
_host(ip),
_port(port),
_configs(conifgs)
{
	// init soket info weith getaddrinfo
	addrinfo hints, *res;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int ret = getaddrinfo(NULL, _port.c_str(), &hints, &res);
	if (ret != 0) {
		throw std::runtime_error(gai_strerror(ret));
	}

	for (addrinfo *rep = res; rep != (addrinfo *)NULL; rep = rep->ai_next) {
		// create socket
		_sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (_sockfd == -1) {
			perror("socket");
			continue;
		}
		// change state of sokcet into nonblocking
		fcntl(_sockfd, F_SETFL, O_NONBLOCK);
		int val = 1;
		setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(val));
		// bind
		if (bind(_sockfd, rep->ai_addr, rep->ai_addrlen) == 0) {
			break;
		}
		perror("bind");
		_sockfd = -1;
		close(_sockfd);
	}
	freeaddrinfo(res);
	if (_sockfd == -1) {
		throw ListenSockError();
	}
	if (::listen(_sockfd, LIESTEN_BACKLOG) == -1) {
		throw ListenSockError("listen");
	}
	getSubject()->subscribe(_sockfd, POLLIN, this);
}

ListenSocket::ListenSocket(
	const ListenSocket & another
)
:IOEventHandler(another.getSubject(), another.getCommandList()),
_host(another._host),
_port(another._port),
_sockfd(another._sockfd),
_configs(another._configs)
{
}

ListenSocket::~ListenSocket()
{
}

int ListenSocket::read()
{
	socklen_t len = sizeof(addrinfo);
	int fd;
	sockaddr_in client_info;

	fd = ::accept(_sockfd, (sockaddr *)&client_info, &len);
	if (fd == -1) {
		throw ListenSockError("accept");
	}
	new AcceptedSocket(getSubject(), getCommandList(), fd, client_info, _configs);
	return 0;
}

int ListenSocket::write()
{
	return 0;
}

void ListenSocket::update(int event)
{
	addCommand(getReadCommand());
}
