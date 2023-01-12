#ifndef IOEVENTHANDLER_HPP
#define IOEVENTHANDLER_HPP

#include "EventHandler.hpp"

class IOEventHandler : public EventHandler
{
public:
	IOEventHandler();
	~IOEventHandler();
	virtual int read() = 0;
	virtual int write() = 0;
};

#endif /* IOEVENTHANDLER_HPP */
