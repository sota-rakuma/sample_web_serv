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

ListenSocket::ListenSockError::~ListenSockError(){};

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
_ip(ip),
_port(port),
_read(new Read(this)),
_configs(conifgs)
{
	// init soket info weith getaddrinfo
	addrinfo hints, *res;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port.c_str(), &hints, &res) == -1) {
		throw std::runtime_error("getaddrinfo");
	}

	int fd;
	for (addrinfo *rep = res; rep != (addrinfo *)NULL; rep = rep->ai_next) {
		// create socket
		fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (fd == -1) {
			perror("socket");
			continue;
		}
		// change state of sokcet into nonblocking
		fcntl(fd, F_SETFL, O_NONBLOCK);
		// bind
		if (bind(fd, rep->ai_addr, rep->ai_addrlen) == 0) {
			break;
		}
		perror("bind");
		fd = -1;
		close(fd);
	}
	freeaddrinfo(res);
	if (fd == -1) {
		throw ListenSockError();
	}
	if (::listen(_sockfd, LIESTEN_BACKLOG) == -1) {
		throw ListenSockError("listen");
	}
	//// listen sock も new して
	//notify(_sockfd, IN, this);
	getSubject()->subscribe(_sockfd, POLLIN, this);
}

ListenSocket::ListenSocket(
	const ListenSocket & another
)
:IOEventHandler(another.getSubject(), another.getCommandList()),
_ip(another._ip),
_port(another._port),
_sockfd(another._sockfd),
_read(new Read(this)),
_configs(another._configs)
{
}

ListenSocket::~ListenSocket()
{
	delete _read;
}

int ListenSocket::read()
{
	socklen_t len = sizeof(addrinfo);
	int fd;
	sockaddr_in client_info;

	if (fd = ::accept(_sockfd, (sockaddr *)&client_info, &len) == -1) {
		throw ListenSockError("accept");
	}
	// acceptedsocketでnotifyしたほうがいいのかもしれない。
	//notify(fd, IN, new AcceptedSocket(getObserver(), fd, client_info, _configs));
	new AcceptedSocket(getSubject(), getCommandList(), fd, client_info, _configs);
	return 0;
}

int ListenSocket::write()
{
	return 0;
}

void ListenSocket::update(int event)
{
	addCommand(_read);
}
