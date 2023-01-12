#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

class IObserver;

class ISubject
{
public:
	ISubject();
	~ISubject();
	virtual void notify(int) = 0;
	virtual void attach(IObserver *) = 0;
	virtual void detach(IObserver * ) = 0;
};

#endif /* ISUBJECT_HPP */
