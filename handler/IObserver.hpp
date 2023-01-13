#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

#include "../command/ICommand.hpp"
#include <list>

class IObserver
{
public:
	IObserver();
	virtual ~IObserver();
	virtual void update(int) = 0;
};

#endif /* IOBSERVER_HPP */