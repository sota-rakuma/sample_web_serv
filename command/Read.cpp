#include "Read.hpp"

Read::Read()
:_receiver(static_cast<AcceptedSocket *>(NULL))
{
}

Read::Read(AcceptedSocket *as)
:_receiver(as)
{
}

Read::~Read()
{
}

int Read::execute()
{
	return _receiver->receive();
}