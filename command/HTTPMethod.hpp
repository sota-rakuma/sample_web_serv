#ifndef HTTPMETHOD_HPP
#define HTTPMETHOD_HPP

#include "ICommand.hpp"
#include "../handler/httphandler/HTTPMethodReceiver.hpp"

class HTTPMethod : public ICommand
{
private:
	HTTPMethodReceiver *_receiver;
public:
	HTTPMethod();
	HTTPMethod(HTTPMethodReceiver *);
	virtual ~HTTPMethod();
	virtual int execute() = 0;
	HTTPMethodReceiver *getReceiver() const;
	HTTPMethod *setReceiver(HTTPMethodReceiver *);
};

#endif /* HTTPMETHOD_HPP */
