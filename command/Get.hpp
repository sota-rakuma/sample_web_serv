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

#endif /* GET_HPP */
