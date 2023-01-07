#ifndef WRITE_HPP
#define WRITE_HPP

#include "ICommand.hpp"
#include "../handler/EventHandler.hpp"

class Write : public ICommand
{
private:
	EventHandler *_reciever;
public:
	Write();
	Write(EventHandler *);
	~Write();
	virtual int execute();
};

Write::Write()
{
}

Write::Write(EventHandler *eh)
:_reciever(eh)
{
}

Write::~Write()
{
}

#endif /* WRITE_HPP */
