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

#endif /* POST_HPP */
