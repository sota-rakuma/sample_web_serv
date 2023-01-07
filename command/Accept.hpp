#ifndef ACCEPT_HPP
#define ACCEPT_HPP

#include "ICommand.hpp"
#include "../subject/ListenSocket.hpp"
#include "../subject/AcceptedSocket.hpp"

class Accept : public ICommand
{
private:
	ListenSocket *_ls;
public:
	Accept();
	Accept(ListenSocket *);
	Accept(const Accept &);
	~Accept();
	virtual int execute();
};

#endif /* ACCEPT_HPP */
