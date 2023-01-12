#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include "../command/ICommand.hpp"
#include "../oldsubject/ISubject.hpp"

class EventHandler : public ISubject
{
public:
	EventHandler();
	EventHandler(IObserver *);
	virtual ~EventHandler();
	virtual ICommand *getHandler(int) const = 0;
};

#endif /* EVENTHANDLER_HPP */
