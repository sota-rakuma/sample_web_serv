#include "ListenSocket.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>

ListenSocket::ListenSocket()
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1) {
		perror("socket");
		std::exit(1);
	}
	if (set_fd_non_blocking()) {
		std::exit(1);
	}
	memset(&_info, 0, sizeof(_info));
	_info.sin_family = AF_INET;
	_info.sin_port = htons(4242);
	_ipaddr = inet_addr("127.0.0.1");
	if (_ipaddr == -1) {
		std::cerr << "cannot find ip address" << std::endl;
		close(_sockfd);
	}
	_info.sin_addr.s_addr = _ipaddr;
	if (bind(_sockfd, (struct sockaddr *)&_info, sizeof(_info))) {
		perror("bind");
		close(_sockfd);
		std::exit(1);
	}
}

ListenSocket::ListenSocket(const ListenSocket& another)
:_ipaddr(another.getIpaddr())
{
	_sockfd = another.getSockFd();
	memset(&_info, 0, sizeof(_info));
	_info.sin_family = another.getSockAddr().sin_family;
	_info.sin_port = another.getSockAddr().sin_port;
	_info.sin_addr.s_addr = _ipaddr;
	if (bind(_sockfd, (struct sockaddr *)&_info, sizeof(_info))) {
		perror("bind");
		close(_sockfd);
		std::exit(1);
	}
}

ListenSocket::ListenSocket(const std::string& ipaddr, short port)
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1) {
		perror("socket");
		std::exit(1);
	}
	if (set_fd_non_blocking()) {
		std::exit(1);
	}
	memset(&_info, 0, sizeof(_info));
	_info.sin_family = AF_INET;
	_info.sin_port = htons(port);
	_ipaddr = inet_addr(ipaddr.c_str());
	if (_ipaddr == -1) {
		std::cerr << "cannot find ip address" << std::endl;
		close(_sockfd);
	}
	_info.sin_addr.s_addr = _ipaddr;
	if (bind(_sockfd, (struct sockaddr *)&_info, sizeof(_info))) {
		perror("bind");
		close(_sockfd);
		std::exit(1);
	}
}

ListenSocket::~ListenSocket()
{
	close(_sockfd);
}

int	ListenSocket::listen_(int backlog)
{
	return listen(_sockfd, backlog);
}

const in_addr_t &ListenSocket::getIpaddr() const {
	return _ipaddr;
}
