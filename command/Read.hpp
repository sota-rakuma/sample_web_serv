#ifndef READ_HPP
#define READ_HPP

#include "ICommand.hpp"
#include "../oldsubject/AcceptedSocket.hpp"
//class AcceptedSocket;

class Read : public ICommand
{
private:
	AcceptedSocket *_receiver;
public:
	Read();
	Read(AcceptedSocket *);
	~Read();
	virtual int execute();
};

#endif /* READ_HPP */
