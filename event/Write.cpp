#include "Write.hpp"

Write::Write()
:_r(static_cast<Reciever *>(NULL))
{
}

Write::Write(Reciever * r)
:_r(r)
{
}

Write::Write(const Write & another)
:_r(another._r)
{
}

EventHandler::Status Write::execute()
{
	if (_r == static_cast<Reciever *>(NULL)) {
		//throw std::runtime_error("RECEIVER is NULL");
		return ERROR;
	}
	return (_r->specialWrite(getFd()));
}

Write::~Write()
{
}
