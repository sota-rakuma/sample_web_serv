#ifndef ISUBSCRIBER_HPP
#define ISUBSCRIBER_HPP

class ISubscriber
{
public:
	ISubscriber();
	virtual ~ISubscriber();
	virtual void update(int) = 0;
};

#endif /* ISUBSCRIBER_HPP */
