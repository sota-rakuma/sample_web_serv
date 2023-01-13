#include "Server.hpp"
#include "handler/ListenSocket.hpp"

Server::Server()
{
}

Server::Server(int backlog)
:_ev(backlog)
{
}

Server::Server(const Server &another)
:_ev(another._ev)
{
}

Server::~Server()
{
}

void Server::run()
{
	_ev.monitor();
}