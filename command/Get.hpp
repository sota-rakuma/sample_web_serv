#ifndef GET_HPP
#define GET_HPP

#include "HTTPMethod.hpp"
#include "../handler/httphandler/HTTPMethodReceiver.hpp"

class Get : public HTTPMethod
{
private:
public:
	Get();
	Get(HTTPMethodReceiver *);
	~Get();
	virtual int execute();
};

#endif /* GET_HPP */
