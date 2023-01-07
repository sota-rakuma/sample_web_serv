#include "Get.hpp"

Get::Get()
:_reciever(static_cast<HTTPMethodReciever *>(NULL))
{
}

Get::Get(HTTPMethodReciever *eh)
:_reciever(eh)
{
}

Get::~Get()
{
}

int Get::execute()
{
	return _reciever->httpGet();
}