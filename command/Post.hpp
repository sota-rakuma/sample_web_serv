#ifndef POST_HPP
#define POST_HPP

#include "ICommand.hpp"
#include "../handler/httphandler/HTTPMethodReceiver.hpp"

class Post : public ICommand
{
private:
	HTTPMethodReceiver *_receiver;
public:
	Post();
	Post(HTTPMethodReceiver *);
	~Post();
	virtual int execute();
	Post & setReceiver(HTTPMethodReceiver *);
};

#endif /* POST_HPP */
