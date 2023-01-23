#ifndef DELETE_HPP
#define DELETE_HPP

#include "ICommand.hpp"
#include "../handler/httphandler/HTTPMethodReceiver.hpp"

class Delete : public ICommand
{
private:
	HTTPMethodReceiver *_receiver;
public:
	Delete();
	Delete(HTTPMethodReceiver *);
	~Delete();
	virtual int execute();
	Delete & setReceiver(HTTPMethodReceiver *);
};

#endif /* DELETE_HPP */
