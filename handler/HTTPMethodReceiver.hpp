#ifndef HTTPMETHODRECIEVER_HPP
#define HTTPMETHODRECIEVER_HPP

enum Stat {
	SUCCESS,
	ERROR,
	CONTINUE,
};

class HTTPMethodReceiver
{
private:
public:
	HTTPMethodReceiver(/* args */);
	~HTTPMethodReceiver();
	virtual int httpGet() = 0;
	virtual int httpPost() = 0;
	virtual int httpDelete() = 0;
};

HTTPMethodReceiver::HTTPMethodReceiver()
{
}

HTTPMethodReceiver::~HTTPMethodReceiver()
{
}


#endif /* HTTPMETHODRECIEVER_HPP */
