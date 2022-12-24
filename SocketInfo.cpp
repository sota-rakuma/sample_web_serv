#include "SocketInfo.hpp"
#include <fcntl.h>

SocketInfo::SocketInfo()
{
	_sockfd = -1;
	memset(&_info, 0, sizeof(_info));
}

SocketInfo::SocketInfo(int sockfd, const sockaddr_in &info)
: _sockfd(sockfd),
_info(info)
{
}

SocketInfo::SocketInfo(const SocketInfo &another)
: _sockfd(another.getSockFd()),
_info(another.getSockAddr())
{
}


SocketInfo::~SocketInfo()
{
}

const int &SocketInfo::getSockFd() const
{
	return _sockfd;
}

const struct sockaddr_in &SocketInfo::getSockAddr() const
{
	return _info;
}

int SocketInfo::set_fd_non_blocking()
{
	int flag = fcntl(_sockfd, F_GETFL);
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1) {
		perror("fcntl");
		return 1;
	}
	return 0;
}
