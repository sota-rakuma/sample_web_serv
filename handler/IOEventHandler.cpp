#include "IOEventHandler.hpp"

IOEventHandler::IOEventHandler()
:_subject(static_cast<ISubject *>(NULL))
{
}
IOEventHandler::IOEventHandler(ISubject *subject)
:_subject(subject)
{
}
IOEventHandler::IOEventHandler(const IOEventHandler &another)
:_subject(another._subject)
{
}
IOEventHandler::~IOEventHandler()
{
}