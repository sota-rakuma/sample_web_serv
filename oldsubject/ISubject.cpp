#include "ISubject.hpp"

ISubject::ISubject()
:_oldobserver(static_cast<IObserver *>(NULL))
{
}

ISubject::ISubject(IObserver * oldobserver)
:_oldobserver(oldobserver)
{
}

ISubject::~ISubject()
{
}

IObserver * ISubject::getObserver() const
{
	return _oldobserver;
}