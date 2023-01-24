#ifndef CGI_HPP
#define CGI_HPP

#include "HTTPMethodReceiver.hpp"
#include "../../command/Read.hpp"
#include "../../command/Write.hpp"
#include "../../HTTP/HTTPStatusCode.hpp"
#include <string>
#include <stdexcept>

class CGI : public HTTPMethodReceiver, public IObserver
{
private:
	int _p_to_c[2];
	int _c_to_p[2];
	std::string _buff;
	size_t _nb;
	bool _is_exutetable;
public:
	CGI();
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		AcceptedSocket *
	);
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		const std::string &,
		bool,
		AcceptedSocket *
	);
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *,
		const std::string &,
		AcceptedSocket *
	);
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		HTTPMethod *,
		const std::string &,
		bool,
		AcceptedSocket *
	);
	CGI(const CGI &);
	virtual ~CGI();
	virtual void update(int);
	virtual int read();
	virtual int write();
	virtual int httpGet();
	virtual int httpPost();
	virtual int httpDelete();
	void executeCGI(const std::string &);
	void setMetaVariables(const std::string &);
	int getInFd() const;
	int getOutFd() const;
	const std::string & getPath() const;
	bool getExectableFlag() const;
	CGI & setExectableFlag(bool);
};

#endif /* CGI_HPP */
