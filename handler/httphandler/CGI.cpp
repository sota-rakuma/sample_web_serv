#include "../AcceptedSocket.hpp"
#include "../../utils/Template.hpp"
#include "CGI.hpp"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#ifndef IN
#define IN 0
#endif

#ifndef OUT
#define OUT 1
#endif

static const std::pair<std::string, std::string> tmp[] = {
	std::make_pair(".perl", "/usr/bin/perl"),
	std::make_pair(".pl", "/usr/bin/perl")
};

const std::map<std::string, std::string> CGI::_commands(tmp, tmp + getSize(tmp));

CGI::CGI(
	ISubject * subject,
	std::list<ICommand *> * commands,
	AcceptedSocket * as,
	const std::vector<std::string> &extensions,
	const std::string & query
)
:HTTPMethodReceiver(subject, commands, as),
_extensions(extensions),
_query(query)
{
}

CGI::CGI(
	ISubject * subject,
	std::list<ICommand *> * commands,
	AcceptedSocket * as,
	const std::vector<std::string> &extensions,
	const std::string & path,
	const std::string & query
)
:HTTPMethodReceiver(subject, commands, as, path),
_extensions(extensions),
_query(query)
{
}

CGI::CGI(
	ISubject * subject,
	std::list<ICommand *> * commands,
	AcceptedSocket * as,
	const std::vector<std::string> &extensions,
	HTTPMethod *method,
	const std::string & path,
	const std::string & query
)
:HTTPMethodReceiver(subject, commands, method, as, path),
_query(query),
_extensions(extensions)
{
}

CGI::CGI(const CGI & another)
:HTTPMethodReceiver(
	another.getSubject(),
	another.getCommandList(),
	another.getHTTPMethod(),
	another.getAcceptedSocket(),
	another.getPath(),
	another.getContent()
),
_extensions(another._extensions),
_query(another._query)
{
}

CGI::~CGI()
{
	int status;
	pid_t ret = ::waitpid(_pid, &status, WNOHANG);
	if (ret == 0) {
		::kill(_pid, SIGKILL);
	}
}

void CGI::update(int event)
{
	if (event & (POLLNVAL | POLLERR)) {
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], true);
	}
	if (event & POLLHUP) {
		entrustCreateResponse(OK);
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], true);
	} else if (event & POLLOUT) {
		getCommandList()->push_back(getWriteCommand());
	} else if (event & POLLIN) {
		std::cout << "read " << std::endl;
		getCommandList()->push_back(getReadCommand());
	}
}

int CGI::read()
{
	char buff[BUFSIZE];
	ssize_t nb = ::read(_c_to_p[IN], buff, BUFSIZE - 1);
	if (nb < 0) {
		// error レスポンス
		::close(_p_to_c[OUT]);
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], true);
		return -1;
	} else if (nb == 0) {
		entrustCreateResponse(OK);
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], true);
		// レスポンス作成フェーズ
		//getAcceptedSocket()->processCGIResponse(_buff);
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	return 1;
}

int CGI::write()
{
	ssize_t nb = ::write(_p_to_c[OUT], _buff.c_str(), _buff.size());
	if (nb == -1) {
		::close(_p_to_c[OUT]);
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		getSubject()->unsubscribe(_c_to_p[IN], true);
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		return -1;
	}
	if (nb < _nb + _buff.size()) {
		_nb += nb;
		_buff = _buff.substr(0, nb);
		return 1;
	}
	::close(_p_to_c[OUT]);
	getSubject()->unsubscribe(_p_to_c[OUT], true);
	_buff = "";
	return 0;
}

int CGI::httpGet()
{
	if (setMetaVariables(GET) == false || isExecutable() == false) {
		return -1;
	}
	if (executeCGI(GET) == false) {
		return -1;
	}
	getSubject()->subscribe(_c_to_p[IN], POLLIN, this);
	setHTTPStatus(OK);
	return 0;
}

int CGI::httpPost()
{
	if (setMetaVariables(POST) == false ||isExecutable() == false) {
		return -1;
	}
	if (executeCGI(POST) == false) {
		return -1;
	}
	//_buff = "value=aaaa&value_2=bbbb";
	getSubject()->subscribe(_p_to_c[OUT], POLLOUT, this);
	getSubject()->subscribe(_c_to_p[IN], POLLIN, this);
	setHTTPStatus(OK);
	return 0;
}

int CGI::httpDelete()
{
	if (setMetaVariables(DELETE) == false || isExecutable() == false) {
		return -1;
	}
	if (executeCGI(DELETE) == false) {
		return -1;
	}
	getSubject()->subscribe(_c_to_p[IN], POLLIN, this);
	entrustCreateResponse(OK);
	return 0;
}

bool CGI::isExecutable()
{
	if (execStat() == -1) {
		if (errno == ENOENT) {
			entrustCreateResponse(NOT_FOUND);
		} else if(errno == EACCES) {
			entrustCreateResponse(FORBIDDEN);
		} else {
			entrustCreateResponse(INTERNAL_SERVER_ERROR);
		}
		return false;
	}
	if (checkPermission(S_IXOTH) == false ||
		isDirectory() == true) {
		entrustCreateResponse(FORBIDDEN);
		return false;
	}
	return true;
}

bool CGI::setMetaVariables(
	const std::string & method
)
{
	size_t extension = std::string::npos;
	for (size_t i = 0; i < _extensions.size(); i++) {
		extension = _path.find(_extensions[i]);
		if (extension != std::string::npos) {
			_command = _extensions[i];
			extension += _extensions[i].size();
			break;
		}
	}
	if (extension == std::string::npos) {
		entrustCreateResponse(FORBIDDEN);
		return false;
	}
	if (setenv("PATH_INFO", _path.substr(extension).c_str(), 1) == -1) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return false;
	}
	if (setenv("QUERY_STRING", _query.c_str(), 1) == -1) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return false;
	}
	_path = _path.substr(0, extension);
	if (setenv("SCRIPT_NAME", _path.c_str(), 1) == -1) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return false;
	}
	if (setenv("REQUEST_METHOD", method.c_str(), 1) == -1) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return false;
	}
	return true;
}

bool CGI::executeCGI(const std::string & method)
{
	if (pipe(_c_to_p) < 0 &&
		(method == POST && pipe(_p_to_c) < 0)) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return false;
	}

	_pid = fork();
	if (_pid < 0) {
		entrustCreateResponse(INTERNAL_SERVER_ERROR);
		return false;
	}
	if (_pid == 0) {
		close(_c_to_p[IN]);
		if (dup2(_p_to_c[OUT], STDOUT_FILENO) == -1)
			exit(1);
		if (method == POST) {
			close(_p_to_c[OUT]);
			if (dup2(_p_to_c[IN], STDIN_FILENO) == -1)
	 			exit(1);
		}
		char * const arg[1] = {const_cast<char *>(getPath().c_str())};
		if (execve(_command.c_str(), arg, environ) == -1) {
			exit(1);
		}
	} else {
		close(_c_to_p[OUT]);
		if (method == POST) {
			close(_p_to_c[IN]);
		}
	}
	return true;
}

int CGI::getInFd() const
{
	return _c_to_p[IN];
}

int CGI::getOutFd() const
{
	return _p_to_c[OUT];
}

void CGI::setQuery(
	const std::string & query
)
{
	_query = query;
}