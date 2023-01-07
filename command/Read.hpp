#ifndef READ_HPP
#define READ_HPP

#include "ICommand.hpp"
//#include "../subject/AcceptedSocket.hpp"
class AcceptedSocket;

class Read : public ICommand
{
private:
	AcceptedSocket *_reciever;
public:
	Read();
	Read(AcceptedSocket *);
	~Read();
	virtual int execute();
};

#endif /* READ_HPP */
