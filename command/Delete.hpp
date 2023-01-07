#ifndef DELETE_HPP
#define DELETE_HPP

#include "ICommand.hpp"
#include "../handler/EventHandler.hpp"

class Delete : public ICommand
{
private:
	EventHandler *_reciever;
public:
	Delete();
	Delete(EventHandler *);
	~Delete();
	virtual int execute();
};

Delete::Delete()
{
}

Delete::Delete(EventHandler *eh)
:_reciever(eh)
{
}

Delete::~Delete()
{
}

#endif /* DELETE_HPP */
