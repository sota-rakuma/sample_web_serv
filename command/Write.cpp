#include "Write.hpp"

Write::Write()
:_receiver(static_cast<AcceptedSocket *>(NULL))
{
}

Write::Write(AcceptedSocket *as)
:_receiver(as)
{
}

Write::~Write()
{
}