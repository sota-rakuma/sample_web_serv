#include "EventHandler.hpp"

EventHandler::EventHandler()
{
}

EventHandler::EventHandler(IObserver *observer)
:ISubject(observer)
{
}

EventHandler::~EventHandler()
{
}