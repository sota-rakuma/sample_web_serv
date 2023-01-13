#ifndef HTTPMETHODRECIEVER_HPP
#define HTTPMETHODRECIEVER_HPP

#include "../IOEventHandler.hpp"

enum Stat {
	SUCCESS,
	ERROR,
	CONTINUE,
};

class HTTPMethodReceiver : public IOEventHandler
{
public:
	HTTPMethodReceiver();
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *
	);
	HTTPMethodReceiver(const HTTPMethodReceiver &);
	virtual ~HTTPMethodReceiver();
	virtual int httpGet() = 0;
	virtual int httpPost() = 0;
	virtual int httpDelete() = 0;
};

#endif /* HTTPMETHODRECIEVER_HPP */
