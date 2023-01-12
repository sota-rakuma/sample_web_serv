#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

#include "../handler/EventHandler.hpp"
#include <string>

#define IN 0x00000001
#define OUT 0x00000002
#define REMOVE 0x00000004

class IObserver
{
public:
	IObserver();
	virtual ~IObserver();
	virtual void update(int, int, EventHandler*) = 0;
};

#endif /* IOBSERVER_HPP */