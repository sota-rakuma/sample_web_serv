#include "HTTPMethodReceiver.hpp"

HTTPMethodReceiver::HTTPMethodReceiver()
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject * subject,
	std::list<ICommand *> * commands
)
:IOEventHandler(subject, commands)
{
}

HTTPMethodReceiver::HTTPMethodReceiver(const HTTPMethodReceiver & another)
:IOEventHandler(another.getSubject(), another.getCommandList())
{
}

HTTPMethodReceiver::~HTTPMethodReceiver()
{
}