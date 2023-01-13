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
