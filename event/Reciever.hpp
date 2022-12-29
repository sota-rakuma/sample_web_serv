#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include "EventHandler.hpp"

class Reciever
{
private:
public:
	Reciever();
	virtual EventHandler::Status specialRead(
		int fd
	) = 0;
	virtual EventHandler::Status specialWrite(
		int fd
	) = 0;
	virtual ~Reciever();
};

Reciever::Reciever()
{
}

Reciever::~Reciever()
{
}


#endif /* RECIEVER_HPP */
