#include "Delete.hpp"

Delete::Delete()
:_reciever(static_cast<HTTPMethodReciever *>(NULL))
{
}

Delete::Delete(HTTPMethodReciever *eh)
:_reciever(eh)
{
}

Delete::~Delete()
{
}

int Delete::execute()
{
	return _reciever->httpDelete();
}
