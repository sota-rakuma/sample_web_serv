#ifndef IOEVENTHANDLER_HPP
#define IOEVENTHANDLER_HPP

#include "../subject/ISubject.hpp"
#include "../utils/utils.hpp"
#include <poll.h>
#include <list>

class Read;
class Write;

class IOEventHandler
{
private:
	ISubject *_subject;
	std::list<ICommand *> *_commands;
	Read *_read;
	Write *_write;
public:
	IOEventHandler();
	IOEventHandler(
		ISubject *,
		std::list<ICommand *> *
	);
	IOEventHandler(const IOEventHandler &);
	ISubject *getSubject() const;
	std::list<ICommand *> * getCommandList() const;
	Read *getReadCommand() const;
	Write *getWriteCommand() const;
	void addCommand(ICommand *);
	virtual ~IOEventHandler();
	virtual int read() = 0;
	virtual int write() = 0;
};

#endif /* IOEVENTHANDLER_HPP */
