#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include "../oldobserver/IObserver.hpp"
#include "../command/ICommand.hpp"

class ISubject
{
private:
	IObserver * _oldobserver;
public:
	ISubject();
	ISubject(IObserver *);
	virtual ~ISubject();
	virtual void notify(int, int, ISubject *) = 0;
	IObserver *getObserver() const;
};

#endif /* ISUBJECT_HPP */
