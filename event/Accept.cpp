#include "Accept.hpp"
#include <sys/socket.h>

Accept::Accept()
:_c()
{
}

Accept::Accept(const Client & c)
:_c(c)
{
}

Accept::Accept(const Accept & another)
:_c(another._c)
{
}

Accept::~Accept()
{
}


// client じゃなくて clientとfdのvectorを持つクラスを渡した方がいい
EventHandler::Status Accept::execute()
{
	sockaddr_in info;
	socklen_t len = sizeof(info);

	int fd = accept(getFd(), (struct sockaddr *)&info, &len);
	if (fd == -1) {
		return ERROR;
	}
	_c.setClientInfo(fd, info);
}