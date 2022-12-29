#include "Client.hpp"

Client::Client()
{
	memset(&_info, 0, sizeof(sockaddr_in));
}

Client::Client(const sockaddr_in & info)
:_info(info)
{
}

Client::Client(
	const sockaddr_in & info,
	const HTTPRequest & req
)
:_info(info),
_req(req)
{
}

Client::Client(
	const sockaddr_in & info,
	const HTTPRequest & req,
	const HTTPResponse & res
)
:_info(info),
_req(req),
_res(res)
{
}

Client::Client(const Client & another)
:_info(another._info),
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

Client & Client::setInfo(
	const sockaddr_in & info
)
{
	_info = info;
}

Client & Client::setRequset(
	const HTTPRequest & req
)
{
	_req = req;
}

Client & Client::setResponse(
	const HTTPResponse & res
)
{
	_res = res;
}
