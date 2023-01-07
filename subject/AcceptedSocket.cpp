#include "AcceptedSocket.hpp"

AcceptedSocket::AcceptedSocket()
:ISubject(),
_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this))
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
_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this)),
_configs(configs)
{
}

AcceptedSocket::AcceptedSocket(const AcceptedSocket &another)
:ISubject(another.getObserver()),
_sockfd(another._sockfd),
_info(another._info),
_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this))
{
}

AcceptedSocket::~AcceptedSocket()
{
	delete _get;
	delete _post;
	delete _delete;
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

// リクエストのchain of responsibility から