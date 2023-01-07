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

#endif /* DELETE_HPP */
