#include "HTTPMethodReceiver.hpp"

HTTPMethodReceiver::HTTPMethodReceiver()
{
}


HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject * subject,
	std::list<ICommand *> * commands
)
:IOEventHandler(subject, commands),
_method(static_cast<HTTPMethod *>(NULL))
{
}

HTTPMethodReceiver::HTTPMethodReceiver(
	ISubject * subject,
	std::list<ICommand *> * commands,
	HTTPMethod *method
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

HTTPMethod *HTTPMethodReceiver::getHTTPMethod() const
{
	return _method;
}

void HTTPMethodReceiver::setHTTPMethod(
	HTTPMethod *method
)
{
	_method = method;
}

bool HTTPMethodReceiver::checkPermission()
{
}