#ifndef CGI_HPP
#define CGI_HPP

#include "HTTPMethodReceiver.hpp"
#include "../../command/Read.hpp"
#include "../../command/Write.hpp"
#include "../../HTTP/HTTPStatusCode.hpp"
#include <string>
#include <stdexcept>
#include <unistd.h>

class CGI : public HTTPMethodReceiver, public IObserver
{
private:
	const std::vector<std::string> & _extentions;
	int _p_to_c[2];
	int _c_to_p[2];
	pid_t _pid;
	std::string _buff;
	size_t _nb;
	std::string _query;
private:
	bool isExecutable();
public:
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		const std::vector<std::string> &,
		const std::string &,
		const std::string &,
		AcceptedSocket *
	);
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		const std::vector<std::string> &,
		HTTPMethod *,
		const std::string &,
		const std::string &,
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
	bool executeCGI(const std::string &);
	bool setMetaVariables(
		const std::string &
	);
	int getInFd() const;
	int getOutFd() const;
	const std::string & getPath() const;
};

#endif /* CGI_HPP */
