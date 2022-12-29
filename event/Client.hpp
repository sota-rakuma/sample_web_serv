#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Reciever.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include <netinet/in.h>
#include <cstring>

class Client : public Reciever
{
private:
	int _sockfd;
	sockaddr_in _info;
	HTTPRequest _req;
	HTTPResponse _res;
public:
	Client();
	Client(
		int ,
		const sockaddr_in &
	);
	Client(
		int ,
		const sockaddr_in &,
		const HTTPRequest &
	);
	Client(
		int ,
		const sockaddr_in &,
		const HTTPRequest &,
		const HTTPResponse &
	);
	Client(const Client &);
	~Client();
	EventHandler::Status specialRead(int fd);
	EventHandler::Status specialWrite(int fd);
	int getSockFd() const;
	const sockaddr_in &getInfo() const;
	const HTTPRequest &getRequset() const;
	const HTTPResponse &getResponse() const;
	Client & setClientInfo(
		int sockfd,
		const sockaddr_in &
	);
	Client & setRequset(
		const HTTPRequest &
	);
	Client & setResponse(
		const HTTPResponse &
	);
};

#endif /* CLIENT_HPP */
