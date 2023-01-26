#include "Write.hpp"
#include "../handler/IOEventHandler.hpp"

Write::Write()
:_receiver(static_cast<IOEventHandler *>(NULL))
{
}

Write::Write(IOEventHandler *handler)
:_receiver(handler)
{
}

Write::~Write()
{
}

int Write::execute()
{
	return _receiver->write();
}