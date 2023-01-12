#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include "../handler/IObserver.hpp"

class ISubject
{
public:
	ISubject();
	virtual ~ISubject();
	virtual void notify(int, int) = 0;
	virtual void subscribe(int, int, IObserver *) = 0;
	virtual void unsubscribe(int, bool, IObserver * ) = 0;
};

#endif /* ISUBJECT_HPP */
