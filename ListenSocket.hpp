#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "SocketInfo.hpp"

class ListenSocket : public SocketInfo
{
private:
	in_addr_t _ipaddr;
public:
	ListenSocket();
	ListenSocket(const ListenSocket& );
	ListenSocket(const std::string&, short);
	~ListenSocket();
	const in_addr_t &getIpaddr() const;
	int	listen_(int);
};

#endif /* LISTENSOCKET_HPP */
