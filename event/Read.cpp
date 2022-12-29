#include "Read.hpp"

Read::Read()
:_r(static_cast<Reciever *>(NULL))
{
}

Read::Read(Reciever * r)
:_r(r)
{
}

Read::Read(
	const Read & another
)
:_r(another._r)
{
}

EventHandler::Status Read::execute()
{
	if (_r == static_cast<Reciever *>(NULL)) {
		//throw std::runtime_error("RECEIVER is NULL");
		return ERROR;
	}
	return (_r->specialRead(getFd()));
}

Read::~Read()
{
}
