#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include "../observer/IObserver.hpp"
#include "../command/ICommand.hpp"
#include "../handler/EventHandler.hpp"

class ISubject : public EventHandler
{
private:
	IObserver * _observer;
public:
	ISubject();
	ISubject(IObserver *);
	virtual ~ISubject();
	virtual void notify(int, int, ISubject *) = 0;
	IObserver *getObserver() const;
};

#endif /* ISUBJECT_HPP */
