#include "Read.hpp"

Read::Read()
:_reciever(static_cast<AcceptedSocket *>(NULL))
{
}

Read::Read(AcceptedSocket *as)
:_reciever(as)
{
}

Read::~Read()
{
}

