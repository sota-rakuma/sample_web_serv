#include "Read.hpp"
#include "../handler/IOEventHandler.hpp"

Read::Read()
:_receiver(static_cast<IOEventHandler *>(NULL))
{
}

Read::Read(IOEventHandler *handler)
:_receiver(handler)
{
}

Read::~Read()
{
}

int Read::execute()
{
	return _receiver->read();
}