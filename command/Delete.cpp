#include "Delete.hpp"

Delete::Delete()
:_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
}

Delete::Delete(HTTPMethodReceiver *eh)
:_receiver(eh)
{
}

Delete::~Delete()
{
}

int Delete::execute()
{
	return _receiver->httpDelete();
}

Delete & Delete::setReceiver(HTTPMethodReceiver *receiver)
{
	_receiver = receiver;
	return *this;
}