#ifndef POST_HPP
#define POST_HPP

#include "ICommand.hpp"
#include "../handler/EventHandler.hpp"

class Post : public ICommand
{
private:
	EventHandler *_reciever;
public:
	Post();
	Post(EventHandler *);
	~Post();
	virtual int execute();
};

Post::Post()
{
}

Post::Post(EventHandler *eh)
:_reciever(eh)
{
}

Post::~Post()
{
}

#endif /* POST_HPP */
