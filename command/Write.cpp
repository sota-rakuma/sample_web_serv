#include "Write.hpp"

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