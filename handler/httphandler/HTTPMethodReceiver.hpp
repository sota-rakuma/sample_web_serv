#ifndef HTTPMETHODRECIEVER_HPP
#define HTTPMETHODRECIEVER_HPP

#include "../IOEventHandler.hpp"
#include "../../HTTP/HTTPStatusCode.hpp"
#include <sys/stat.h>
#include <string>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

class HTTPMethod;
class AcceptedSocket;

class HTTPMethodReceiver : public IOEventHandler
{
private:
	HTTPMethod *_method;
	AcceptedSocket *_as;
	struct stat _state;
	struct stat _parent;
protected:
	std::string _path;
	std::string _query;
	std::string _buff;
public:
	HTTPMethodReceiver();
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *
	);
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *,
		const std::string &
	);
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *,
		const std::string &,
		const std::string &
	);
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *,
		AcceptedSocket *,
		const std::string &
	);
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *,
		AcceptedSocket *,
		const std::string &,
		const std::string &
	);
	HTTPMethodReceiver(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *,
		AcceptedSocket *,
		const std::string &,
		const std::string &,
		const std::string &
	);
	HTTPMethodReceiver(const HTTPMethodReceiver &);
	virtual ~HTTPMethodReceiver();
	HTTPMethod *getHTTPMethod() const;
	AcceptedSocket *getAcceptedSocket() const;
	const std::string & getPath() const;
	const std::string & getQuery() const;
	void setHTTPMethod(HTTPMethod *);
	void setAcceptedSocket(AcceptedSocket *);
	void setPath(const std::string &);
	void setQuery(const std::string &);
	void setContent(
		const std::string &
	);
	void addContent(
		const std::string &
	);
	const std::string & getContent() const;
	virtual int httpGet() = 0;
	virtual int httpPost() = 0;
	virtual int httpDelete() = 0;
	int execStat();
	bool isDirectory() const;
	bool isRegularFile() const;
	bool checkPermission(mode_t) const;
	bool checkPermissionOfParent(mode_t) const;
	int execStatForParentDir();
	void setHTTPStatus(HTTPStatus);
	void entrustCreateResponse(HTTPStatus);
	void entrustCreateResponse();
};

#endif /* HTTPMETHODRECIEVER_HPP */
