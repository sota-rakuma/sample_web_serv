#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include "../command/ICommand.hpp"

class EventHandler
{
public:
	EventHandler();
	virtual ~EventHandler();
	virtual ICommand *getHandler(int) const = 0;
};

#endif /* EVENTHANDLER_HPP */
