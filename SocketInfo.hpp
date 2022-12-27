#ifndef SOCKETINFO_HPP
#define SOCKETINFO_HPP

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>

enum stat {
	ERROR = -1,
	TERMINATED,
	IN_PREPARATION,
	CONTINUE,
};

class SocketInfo
{
protected:
	int _sockfd;
	sockaddr_in _info;
public:
	SocketInfo();
	SocketInfo(int , const sockaddr_in &);
	SocketInfo(const SocketInfo &);
	virtual ~SocketInfo();
	const int &getSockFd() const;
	const struct sockaddr_in &getSockAddr() const;
	int set_fd_non_blocking();
};

#endif /* SOCKETINFO_HPP */
