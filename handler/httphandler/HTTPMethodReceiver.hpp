#ifndef HTTPMETHODRECIEVER_HPP
#define HTTPMETHODRECIEVER_HPP

#include "../IOEventHandler.hpp"
#include "../../command/HTTPMethod.hpp"

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

class HTTPMethodReceiver : public IOEventHandler
{
private:
	HTTPMethod *_method;
public:
	HTTPMethodReceiver();
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *
	);
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *method
	);
	HTTPMethodReceiver(const HTTPMethodReceiver &);
	virtual ~HTTPMethodReceiver();
	HTTPMethod *getHTTPMethod() const;
	void setHTTPMethod(HTTPMethod *);
	virtual int httpGet() = 0;
	virtual int httpPost() = 0;
	virtual int httpDelete() = 0;
	bool checkPermission();
};

#endif /* HTTPMETHODRECIEVER_HPP */
