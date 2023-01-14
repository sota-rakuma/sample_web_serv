#ifndef IOEVENTHANDLER_HPP
#define IOEVENTHANDLER_HPP

#include "../subject/ISubject.hpp"
#include "../command/ICommand.hpp"
#include <poll.h>
#include <list>
class IOEventHandler
{
private:
	ISubject *_subject;
	std::list<ICommand *> *_commands;
public:
	IOEventHandler();
	IOEventHandler(
		ISubject *,
		std::list<ICommand *> *
	);
	IOEventHandler(const IOEventHandler &);
	ISubject *getSubject() const;
	std::list<ICommand *> * getCommandList() const;
	void addCommand(ICommand *);
	virtual ~IOEventHandler();
	virtual int read() = 0;
	virtual int write() = 0;
};

#endif /* IOEVENTHANDLER_HPP */
