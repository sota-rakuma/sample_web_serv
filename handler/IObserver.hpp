#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

class IObserver
{
public:
	IObserver();
	virtual ~IObserver();
	virtual void update(int) = 0;
};

#endif /* IOBSERVER_HPP */
