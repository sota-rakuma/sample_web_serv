#include "Client.hpp"

Client::Client()
:_sockfd(),
_req(),
_res()
{
	memset(&_info, 0, sizeof(sockaddr_in));
}

Client::Client(
	int sockfd,
	const sockaddr_in & info
)
:_sockfd(sockfd),
_info(info)
{
}

Client::Client(
	int sockfd,
	const sockaddr_in & info,
	const HTTPRequest & req
)
:_sockfd(sockfd),
_info(info),
_req(req)
{
}

Client::Client(
	int sockfd,
	const sockaddr_in & info,
	const HTTPRequest & req,
	const HTTPResponse & res
)
:_sockfd(sockfd),
_info(info),
_req(req),
_res(res)
{
}

Client::Client(const Client & another)
:_sockfd(another._sockfd),
_info(another._info),
_req(another._req),
_res(another._res)
{
}

Client::~Client()
{
}


EventHandler::Status Client::specialRead(int fd)
{
}

EventHandler::Status Client::specialWrite(int fd)
{
}



int Client::getSockFd() const
{
	return _sockfd;
}

const sockaddr_in &Client::getInfo() const
{
	return _info;
}

const HTTPRequest &Client::getRequset() const
{
	return _req;
}

const HTTPResponse &Client::getResponse() const
{
	return _res;
}

Client & Client::setClientInfo(
	int sockfd,
	const sockaddr_in & info
)
{
	_sockfd = sockfd;
	_info = info;
	return *this;
}

Client & Client::setRequset(
	const HTTPRequest & req
)
{
	_req = req;
	return *this;
}

Client & Client::setResponse(
	const HTTPResponse & res
)
{
	_res = res;
	return *this;
}
