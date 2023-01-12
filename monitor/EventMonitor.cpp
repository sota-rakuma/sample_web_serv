#include "EventMonitor.hpp"
#include <cstddef>

EventMonitor::EventMonitor()
:_time(300),
_pub(static_cast<IPublisher *>(NULL))
{
}

EventMonitor::EventMonitor(int time)
:_time(time)
{
}

EventMonitor::EventMonitor(IPublisher *pub)
:_pub(pub)
{
}

EventMonitor::EventMonitor(int time, IPublisher * pub)
:_time(time),
_pub(pub)
{
}


EventMonitor::EventMonitor(const EventMonitor &another)
:_time(another._time),
_pub(another._pub),
_pollvec(another._pollvec)
{
}

EventMonitor::~EventMonitor()
{
}


int monitor();