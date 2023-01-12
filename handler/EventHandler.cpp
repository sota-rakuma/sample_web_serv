#include "EventHandler.hpp"

EventHandler::EventHandler()
{
}

EventHandler::EventHandler(IObserver *oldobserver)
:ISubject(oldobserver)
{
}

EventHandler::~EventHandler()
{
}