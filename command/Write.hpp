#ifndef WRITE_HPP
#define WRITE_HPP

#include "ICommand.hpp"

class IOEventHandler;

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
