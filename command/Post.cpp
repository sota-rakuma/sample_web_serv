#include "Post.hpp"

Post::Post()
:_reciever(static_cast<HTTPMethodReciever *>(NULL))
{
}

Post::Post(HTTPMethodReciever *eh)
:_reciever(eh)
{
}

Post::~Post()
{
}

int Post::execute()
{
	return _reciever->httpPost();
}
