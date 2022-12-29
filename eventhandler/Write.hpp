#ifndef WRITE_HPP
#define WRITE_HPP

#include "EventHandler.hpp"
#include "Reciever.hpp"

class Write : public EventHandler
{
private:
	Reciever *_r;
public:
	Write();
	Write(Reciever * r);
	virtual Status execute();
	~Write();
};

#endif /* WRITE_HPP */
