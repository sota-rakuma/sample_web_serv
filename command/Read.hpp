#ifndef READ_HPP
#define READ_HPP

#include "ICommand.hpp"
#include "../handler/EventHandler.hpp"

class Read : public ICommand
{
private:
	EventHandler *_reciever;
public:
	Read();
	Read(EventHandler *);
	~Read();
	virtual int execute();
};

Read::Read()
{
}

Read::Read(EventHandler *eh)
:_reciever(eh)
{
}

Read::~Read()
{
}


#endif /* READ_HPP */
