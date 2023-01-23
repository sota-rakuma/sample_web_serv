#include "Post.hpp"

Post::Post()
:HTTPMethod()
{
}

Post::Post(HTTPMethodReceiver *eh)
:HTTPMethod(eh)
{
}

Post::~Post()
{
}

int Post::execute()
{
	return getReceiver()->httpPost();
}

