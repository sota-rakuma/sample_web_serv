#include "ISubject.hpp"

ISubject::ISubject()
:_observer(static_cast<IObserver *>(NULL))
{
}

ISubject::ISubject(IObserver * observer)
:_observer(observer)
{
}

ISubject::~ISubject()
{
}

IObserver * ISubject::getObserver() const
{
	return _observer;
}