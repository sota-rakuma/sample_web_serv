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
_commands(commands)
{
}

IOEventHandler::IOEventHandler(const IOEventHandler &another)
:_subject(another._subject),
_commands(another._commands)
{
}

IOEventHandler::~IOEventHandler()
{
}

ISubject *IOEventHandler::getSubject() const
{
	return _subject;
}

std::list<ICommand *> *IOEventHandler::getCommandList() const
{
	return _commands;
}
