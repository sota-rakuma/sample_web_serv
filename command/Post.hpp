#ifndef POST_HPP
#define POST_HPP

#include "ICommand.hpp"
#include "../handler/HTTPMethodReciever.hpp"

class Post : public ICommand
{
private:
	HTTPMethodReciever *_reciever;
public:
	Post();
	Post(HTTPMethodReciever *);
	~Post();
	virtual int execute();
};

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

#endif /* POST_HPP */
