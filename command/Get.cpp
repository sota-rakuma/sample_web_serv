#include "Get.hpp"

Get::Get()
:HTTPMethod()
{
}

Get::Get(HTTPMethodReceiver *eh)
:HTTPMethod(eh)
{
}

Get::~Get()
{
}

int Get::execute()
{
	return getReceiver()->httpGet();
}
