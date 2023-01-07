#include "Write.hpp"

Write::Write()
:_reciever(static_cast<AcceptedSocket *>(NULL))
{
}

Write::Write(AcceptedSocket *as)
:_reciever(as)
{
}

Write::~Write()
{
}