#ifndef WRITE_HPP
#define WRITE_HPP

#include "ICommand.hpp"
#include "../oldsubject/AcceptedSocket.hpp"
//class AcceptedSocket;

class Write : public ICommand
{
private:
	AcceptedSocket *_receiver;
public:
	Write();
	Write(AcceptedSocket *);
	~Write();
	virtual int execute();
};

#endif /* WRITE_HPP */
