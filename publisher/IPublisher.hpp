#ifndef IPUBLISHER_HPP
#define IPUBLISHER_HPP

class ISubscriber;

class IPublisher
{
public:
	IPublisher();
	virtual ~IPublisher();
	virtual void notify(int) = 0;
	virtual void subscribe(ISubscriber *) = 0;
	virtual void unsubscribe(ISubscriber * ) = 0;
};

#endif /* IPUBLISHER_HPP */
