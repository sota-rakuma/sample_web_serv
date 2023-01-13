#include "Get.hpp"

Get::Get()
:_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
}

Get::Get(HTTPMethodReceiver *eh)
:_receiver(eh)
{
}

Get::~Get()
{
}

int Get::execute()
{
	return _receiver->httpGet();
}