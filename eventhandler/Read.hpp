#ifndef READ_HPP
#define READ_HPP

#include "EventHandler.hpp"
#include "Reciever.hpp"

class Read : public EventHandler
{
private:
	Reciever *_r;
public:
	Read();
	Read(Reciever * r);
	virtual Status execute();
	~Read();
};


#endif /* READ_HPP */
