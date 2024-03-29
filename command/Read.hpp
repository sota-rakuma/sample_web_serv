#ifndef READ_HPP
#define READ_HPP

#include "ICommand.hpp"

class IOEventHandler;

class Read : public ICommand
{
private:
	IOEventHandler *_receiver;
public:
	Read();
	Read(IOEventHandler  *);
	~Read();
	virtual int execute();
};

#endif /* READ_HPP */
