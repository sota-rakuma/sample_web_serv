#include "Delete.hpp"

Delete::Delete()
:HTTPMethod()
{
}

Delete::Delete(HTTPMethodReceiver *eh)
:HTTPMethod(eh)
{
}

Delete::~Delete()
{
}

int Delete::execute()
{
	return getReceiver()->httpDelete();
}
