#ifndef WRITE_HPP
#define WRITE_HPP

#include "ICommand.hpp"
//#include "../subject/AcceptedSocket.hpp"
class AcceptedSocket;

class Write : public ICommand
{
private:
	AcceptedSocket *_reciever;
public:
	Write();
	Write(AcceptedSocket *);
	~Write();
	virtual int execute();
};

#endif /* WRITE_HPP */
