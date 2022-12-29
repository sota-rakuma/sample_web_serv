#ifndef ACCEPT_HPP
#define ACCEPT_HPP

#include "EventHandler.hpp"
#include "Client.hpp"

class Accept : public EventHandler
{
private:
	Client _c;
public:
	Accept();
	Accept(const Client &);
	Accept(const Accept &);
	~Accept();
	virtual Status execute();
};

#endif /* ACCEPT_HPP */
