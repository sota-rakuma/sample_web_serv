#ifndef IOEVENTHANDLER_HPP
#define IOEVENTHANDLER_HPP

#include "EventHandler.hpp"
#include "../subject/ISubject.hpp"
#include <list>
#include "../command/ICommand.hpp"

class IOEventHandler : public EventHandler
{
private:
	ISubject *_subject;
	std::list<ICommand *> *_commads;
public:
	IOEventHandler();
	IOEventHandler(
		ISubject *,
		std::list<ICommand *> *
	);
	IOEventHandler(const IOEventHandler &);
	ISubject *getSubject() const;
	~IOEventHandler();
	virtual int read() = 0;
	virtual int write() = 0;
};

#endif /* IOEVENTHANDLER_HPP */
