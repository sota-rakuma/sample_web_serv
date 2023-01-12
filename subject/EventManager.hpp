#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "ISubject.hpp"
#include "../command/ICommand.hpp"
#include <map>
#include <list>

class EventManager : public ISubject
{
private:
	std::map<int, ISubscriber *> _storage;
public:
	EventManager(/* args */);
	~EventManager();
};

EventManager::EventManager(/* args */)
{
}

EventManager::~EventManager()
{
}


#endif /* EVENTMANAGER_HPP */
