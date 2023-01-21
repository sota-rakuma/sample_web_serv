#include "HTTPMethodReceiver.hpp"

HTTPMethodReceiver::HTTPMethodReceiver()
{
}


HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject * subject,
	std::list<ICommand *> * commands
)
:IOEventHandler(subject, commands),
_method(static_cast<ICommand *>(NULL))
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject * subject,
	std::list<ICommand *> * commands,
	ICommand *method
)
:IOEventHandler(subject, commands),
_method(method)
{
}

HTTPMethodReceiver::HTTPMethodReceiver(const HTTPMethodReceiver & another)
:IOEventHandler(another.getSubject(), another.getCommandList()),
_method(another._method)
{
}

HTTPMethodReceiver::~HTTPMethodReceiver()
{
	delete _method;
}

ICommand *HTTPMethodReceiver::getHTTPMethod() const
{
	return _method;
}

void HTTPMethodReceiver::setHTTPMethod(
	ICommand *method
)
{
	_method = method;
}