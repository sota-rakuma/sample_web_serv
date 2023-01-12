#ifndef IOEVENTHANDLER_HPP
#define IOEVENTHANDLER_HPP

#include "EventHandler.hpp"
#include "../subject/ISubject.hpp"

class IOEventHandler : public EventHandler
{
private:
	ISubject *_subject;
public:
	IOEventHandler();
	IOEventHandler(ISubject *);
	IOEventHandler(const IOEventHandler &);
	~IOEventHandler();
	virtual int read() = 0;
	virtual int write() = 0;
};

#endif /* IOEVENTHANDLER_HPP */
