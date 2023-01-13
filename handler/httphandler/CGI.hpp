#ifndef CGI_HPP
#define CGI_HPP

#include "HTTPMethodReceiver.hpp"
#include "../AcceptedSocket.hpp"
#include "../../command/Read.hpp"
#include "../../command/Write.hpp"
#include "../../HTTP/HTTPStatusCode.hpp"
#include <string>
#include <stdexcept>

class CGI : public HTTPMethodReceiver, public IObserver
{
private:
	int _in_fd;
	int _out_fd;
	std::string _buff;
	size_t _nb;
	bool _is_exutetable;
	HTTPStatus _status;
	std::string _path;
	Read *_read;
	Write *_write;
	AcceptedSocket *_as;
public:
	CGI();
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &
	);
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		bool
	);
	CGI(const CGI &);
	virtual ~CGI();
	virtual void update(int);
	virtual int read();
	virtual int write();
	virtual int httpGet();
	virtual int httpPost();
	virtual int httpDelete();
	virtual ICommand *getHandler(int) const;
	void executeCGI(HTTPMethod);
	void setMetaVariables(HTTPMethod);
	int getInFd() const;
	int getOutFd() const;
	const std::string & getPath() const;
	bool getExectableFlag() const;
	CGI & setExectableFlag(bool);
};

#endif /* CGI_HPP */
