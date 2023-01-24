#ifndef DELETE_HPP
#define DELETE_HPP

#include "HTTPMethod.hpp"
#include "../handler/httphandler/HTTPMethodReceiver.hpp"

class Delete : public HTTPMethod
{
private:
public:
	Delete();
	Delete(HTTPMethodReceiver *);
	~Delete();
	virtual int execute();
};

#endif /* DELETE_HPP */
