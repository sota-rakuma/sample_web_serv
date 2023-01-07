#ifndef GET_HPP
#define GET_HPP

#include "ICommand.hpp"
#include "../handler/HTTPMethodReciever.hpp"

class Get : public ICommand
{
private:
	HTTPMethodReciever *_reciever;
public:
	Get();
	Get(HTTPMethodReciever *);
	~Get();
	virtual int execute();
};

Get::Get()
:_reciever(static_cast<HTTPMethodReciever *>(NULL))
{
}

Get::Get(HTTPMethodReciever *eh)
:_reciever(eh)
{
}

Get::~Get()
{
}

int Get::execute()
{
	return _reciever->httpGet();
}

#endif /* GET_HPP */
