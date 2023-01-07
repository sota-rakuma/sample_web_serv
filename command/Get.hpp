#ifndef GET_HPP
#define GET_HPP

#include "ICommand.hpp"
#include "../handler/EventHandler.hpp"

class Get : public ICommand
{
private:
	EventHandler *_reciever;
public:
	Get();
	Get(EventHandler *);
	~Get();
	virtual int execute();
};

Get::Get()
{
}

Get::Get(EventHandler *eh)
:_reciever(eh)
{
}

Get::~Get()
{
}

#endif /* GET_HPP */
