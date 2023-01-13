#ifndef GET_HPP
#define GET_HPP

#include "ICommand.hpp"
#include "../handler/httphandler/HTTPMethodReceiver.hpp"

class Get : public ICommand
{
private:
	HTTPMethodReceiver *_receiver;
public:
	Get();
	Get(HTTPMethodReceiver *);
	~Get();
	virtual int execute();
};

#endif /* GET_HPP */
