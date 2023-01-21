#ifndef HTTPMETHODRECIEVER_HPP
#define HTTPMETHODRECIEVER_HPP

#include "../IOEventHandler.hpp"
#include "../../command/ICommand.hpp"

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

class HTTPMethodReceiver : public IOEventHandler
{
private:
	ICommand *_method;
public:
	HTTPMethodReceiver();
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		ICommand *method
	);
	HTTPMethodReceiver(const HTTPMethodReceiver &);
	virtual ~HTTPMethodReceiver();
	ICommand *getHTTPMethod() const;
	virtual int httpGet() = 0;
	virtual int httpPost() = 0;
	virtual int httpDelete() = 0;
};

#endif /* HTTPMETHODRECIEVER_HPP */
