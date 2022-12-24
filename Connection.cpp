#include "Connection.hpp"

#ifndef BUFFSIZE
#define BUFFSIZE 1024
#endif

Connection::Connection()
:SocketInfo()
{
}

Connection::Connection(const SocketInfo & socketinfo)
:SocketInfo(socketinfo)
{
}

Connection::Connection(int sockfd, const sockaddr_in & client)
:SocketInfo(sockfd, client)
{
}

Connection::Connection
(int sockfd, const sockaddr_in & client, const std::string & rbuff)
:SocketInfo(sockfd, client),
_rbuff(rbuff)
{
}

Connection::Connection
(int sockfd, const sockaddr_in & client, const std::string & rbuff, const std::string & wbuff)
:SocketInfo(sockfd, client),
_rbuff(rbuff),
_wbuff(wbuff)
{
}

Connection::Connection(const Connection& another)
: SocketInfo(another.getSockFd(), another.getSockAddr()),
_rbuff(another.getRbuffer()),
_wbuff(another.getWbuffer())
{
}

Connection::~Connection()
{
	if (close(_sockfd)) {
		perror("close");
	}
}

const std::string &Connection::getRbuffer() const
{
	return _rbuff;
}

const std::string &Connection::getWbuffer() const
{
	return _wbuff;
}

int Connection::getRequest()
{
	char buff[BUFFSIZE];
	ssize_t rc;

	for (;;) {
		rc = recv(_sockfd, buff, BUFFSIZE - 1, MSG_DONTWAIT);
		if (rc < 0) {
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				perror("recv");
				return ERROR;
			}
			return IN_PREPARATION;
		} else if (rc == 0) {
			return TERMINATED;
		}
		buff[rc] = '\0';
		_rbuff += buff;
	}
	return CONTINUE;
}

// 一旦
int Connection::createResponse()
{
	_wbuff = _rbuff;
	return TERMINATED;
}

int Connection::sendResponse()
{
	std::string buff;
	ssize_t	wc;

	for (;;) {
		if (_wbuff.size() >= BUFFSIZE) {
			buff = _wbuff.substr(0, BUFFSIZE - 1);
		} else {
			buff = _wbuff;
		}
		wc = send(_sockfd, buff.c_str(), buff.size(), MSG_DONTWAIT);
		if (wc < 0) {
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				perror("send");
				return ERROR;
			}
			return IN_PREPARATION;
		} else if (wc == _wbuff.size()) {
			return TERMINATED;
		}
		_wbuff = _wbuff.substr(wc, _wbuff.size() - wc + 1);
	}
	return CONTINUE;
}
