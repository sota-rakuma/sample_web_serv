#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include "../handler/IObserver.hpp"

class ISubject
{
public:
	ISubject();
	virtual ~ISubject();
	virtual void notify(int, std::list<ICommand *> &) = 0;
	virtual void subscribe(IObserver *) = 0;
	virtual void unsubscribe(IObserver * ) = 0;
};

#endif /* ISUBJECT_HPP */
