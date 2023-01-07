#ifndef HTTPMETHODRECIEVER_HPP
#define HTTPMETHODRECIEVER_HPP

#include "EventHandler.hpp"

enum Stat {
	SUCCESS,
	ERROR,
	CONTINUE,
};

class HTTPMethodReciever : public EventHandler
{
private:
public:
	HTTPMethodReciever(/* args */);
	~HTTPMethodReciever();
	virtual int httpGet() = 0;
	virtual int httpPost() = 0;
	virtual int httpDelete() = 0;
};

HTTPMethodReciever::HTTPMethodReciever()
{
}

HTTPMethodReciever::~HTTPMethodReciever()
{
}


#endif /* HTTPMETHODRECIEVER_HPP */
