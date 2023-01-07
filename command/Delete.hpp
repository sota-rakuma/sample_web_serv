#ifndef DELETE_HPP
#define DELETE_HPP

#include "ICommand.hpp"
#include "../handler/HTTPMethodReciever.hpp"

class Delete : public ICommand
{
private:
	HTTPMethodReciever *_reciever;
public:
	Delete();
	Delete(HTTPMethodReciever *);
	~Delete();
	virtual int execute();
};

Delete::Delete()
:_reciever(static_cast<HTTPMethodReciever *>(NULL))
{
}

Delete::Delete(HTTPMethodReciever *eh)
:_reciever(eh)
{
}

Delete::~Delete()
{
}

#endif /* DELETE_HPP */
