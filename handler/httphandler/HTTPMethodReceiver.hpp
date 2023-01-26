#ifndef HTTPMETHODRECIEVER_HPP
#define HTTPMETHODRECIEVER_HPP

#include "../IOEventHandler.hpp"
#include "../../command/HTTPMethod.hpp"
#include <sys/stat.h>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

class AcceptedSocket;

class HTTPMethodReceiver : public IOEventHandler
{
private:
	HTTPMethod *_method;
	AcceptedSocket *_as;
	struct stat _state;
protected:
	std::string _path;
public:
	HTTPMethodReceiver();
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *,
		const std::string &
	);
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *,
		AcceptedSocket *,
		const std::string &
	);
	HTTPMethodReceiver(const HTTPMethodReceiver &);
	virtual ~HTTPMethodReceiver();
	HTTPMethod *getHTTPMethod() const;
	AcceptedSocket *getAcceptedSocket() const;
	const std::string & getPath() const;
	void setHTTPMethod(HTTPMethod *);
	void setAcceptedSocket(AcceptedSocket *);
	void setPath(const std::string &);
	virtual int httpGet() = 0;
	virtual int httpPost() = 0;
	virtual int httpDelete() = 0;
	int execStat();
	bool isDirectory() const;
	bool isRegularFile() const;
	bool checkPermission(mode_t) const;
	void setHTTPStatus(HTTPStatus);
	void entrustCreateResponse(HTTPStatus);
};

#endif /* HTTPMETHODRECIEVER_HPP */
