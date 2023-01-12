#ifndef IOEVENTHANDLER_HPP
#define IOEVENTHANDLER_HPP

#include "EventHandler.hpp"
#include "../subject/ISubject.hpp"
#include "../command/ICommand.hpp"
#include <list>
#define IN 0x00000001
#define OUT 0x00000002

class IOEventHandler : public EventHandler
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
	~IOEventHandler();
	virtual int read() = 0;
	virtual int write() = 0;
};

#endif /* IOEVENTHANDLER_HPP */
