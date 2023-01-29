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
	static const std::map<std::string, std::string> _commands;
	const std::vector<std::string> & _extensions;
	int _p_to_c[2];
	int _c_to_p[2];
	pid_t _pid;
	size_t _nb;
	std::string _command;
private:
	bool isExecutable();
	bool executeCGI(const std::string &);
	bool setMetaVariables(
		const std::string &
	);
public:
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *,
		const std::vector<std::string> &
	);
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *,
		const std::vector<std::string> &,
		const std::string &,
		const std::string &
	);
	CGI(
		ISubject *,
		std::list<ICommand *> *,
		AcceptedSocket *,
		const std::vector<std::string> &,
		HTTPMethod *,
		const std::string &,
		const std::string &
	);
	CGI(const CGI &);
	virtual ~CGI();
	virtual void update(int);
	virtual int read();
	virtual int write();
	virtual int httpGet();
	virtual int httpPost();
	virtual int httpDelete();
	int getInFd() const;
	int getOutFd() const;
	void setQuery(
		const std::string &
	);
};

#endif /* CGI_HPP */
