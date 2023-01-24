#ifndef POST_HPP
#define POST_HPP

#include "HTTPMethod.hpp"
#include "../handler/httphandler/HTTPMethodReceiver.hpp"

class Post : public HTTPMethod
{
private:
public:
	Post();
	Post(HTTPMethodReceiver *);
	~Post();
	virtual int execute();
};

#endif /* POST_HPP */
