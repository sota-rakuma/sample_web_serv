#include "AcceptedSocket.hpp"

AcceptedSocket::AcceptedSocket()
:ISubject()
{
}

AcceptedSocket::AcceptedSocket(
	IObserver *observer,
	int sockfd,
	const sockaddr_in &info,
	ServerConfigFinder *configs
)
:ISubject(observer),
_sockfd(sockfd),
_info(info),
_configs(configs)
{
}

AcceptedSocket::AcceptedSocket(const AcceptedSocket &another)
:ISubject(another.getObserver()),
_sockfd(another._sockfd),
_info(another._info)
{
}

AcceptedSocket::~AcceptedSocket()
{
}

AcceptedSocket &AcceptedSocket::setInfo(const sockaddr_in & info)
{
	_info = info;
	return *this;
}

AcceptedSocket &AcceptedSocket::setFd(int sockfd)
{
	_sockfd = sockfd;
	return *this;
}

void AcceptedSocket::notify(
	int fd,
	int event,
	ISubject *subject
)
{
	getObserver()->update(fd, event, subject);
}

// リクエストのchain of responsibility から