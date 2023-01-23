#include "Post.hpp"

Post::Post()
:_receiver(static_cast<HTTPMethodReceiver *>(NULL))
{
}

Post::Post(HTTPMethodReceiver *eh)
:_receiver(eh)
{
}

Post::~Post()
{
}

int Post::execute()
{
	return _receiver->httpPost();
}

Post & Post::setReceiver(HTTPMethodReceiver * receiver)
{
	_receiver = receiver;
	return *this;
}
