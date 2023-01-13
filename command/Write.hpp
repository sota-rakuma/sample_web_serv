#ifndef WRITE_HPP
#define WRITE_HPP

#include "ICommand.hpp"
#include "../handler/IOEventHandler.hpp"

class Write : public ICommand
{
private:
	IOEventHandler *_receiver;
public:
	Write();
	Write(IOEventHandler *);
	~Write();
	virtual int execute();
};

#endif /* WRITE_HPP */
