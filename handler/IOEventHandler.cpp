#include "IOEventHandler.hpp"

IOEventHandler::IOEventHandler()
:_subject(static_cast<ISubject *>(NULL))
{
}
IOEventHandler::IOEventHandler(
	ISubject *subject,
	std::list<ICommand *> *commands
)
:_subject(subject),
_commands(commands),
_read(new Read(this)),
_write(new Write(this))
{
}

IOEventHandler::IOEventHandler(const IOEventHandler &another)
:_subject(another._subject),
_commands(another._commands),
_read(new Read(this)),
_write(new Write(this))
{
}

IOEventHandler::~IOEventHandler()
{
	delete _read;
	delete _write;
}

ISubject *IOEventHandler::getSubject() const
{
	return _subject;
}

std::list<ICommand *> *IOEventHandler::getCommandList() const
{
	return _commands;
}

Read *IOEventHandler::getReadCommand() const
{
	return _read;
}

Write *IOEventHandler::getWriteCommand() const
{
	return _write;
}

void IOEventHandler::addCommand(ICommand * command)
{
	_commands->push_back(command);
}
