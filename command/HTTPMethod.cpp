#include "../handler/httphandler/HTTPMethodReceiver.hpp"
#include "HTTPMethod.hpp"

HTTPMethod::HTTPMethod()
:_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
}

HTTPMethod::HTTPMethod(HTTPMethodReceiver *receiver)
:_receiver(receiver)
{
}

HTTPMethod::~HTTPMethod()
{
}

HTTPMethodReceiver *HTTPMethod::getReceiver() const
{
	return _receiver;
}

HTTPMethod *HTTPMethod::setReceiver(HTTPMethodReceiver *receiver)
{
	_receiver = receiver;
	return this;
}
