#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include "../observer/IObserver.hpp"

class ISubject
{
public:
	ISubject();
	virtual ~ISubject();
	virtual void notify(int) = 0;
	virtual void subscribe(IObserver *) = 0;
	virtual void unsubscribe(IObserver * ) = 0;
};

#endif /* ISUBJECT_HPP */
