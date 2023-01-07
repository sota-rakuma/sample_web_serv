#include "Accept.hpp"

Accept::Accept()
:_ls(static_cast<ListenSocket *>(NULL))
{
}

Accept::Accept(ListenSocket * ls)
:_ls(ls)
{
}

Accept::Accept(const Accept &another)
:_ls(another._ls)
{
}

Accept::~Accept()
{
}

int Accept::execute()
{
	_ls->accept();
	return 0;
}

