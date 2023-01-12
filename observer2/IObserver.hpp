#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

class IObserver
{
public:
	IObserver();
	~IObserver();
	virtual void update(int) = 0;
};

IObserver::IObserver()
{
}

IObserver::~IObserver()
{
}


#endif /* IOBSERVER_HPP */
