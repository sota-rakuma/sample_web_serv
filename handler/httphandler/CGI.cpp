#include "../AcceptedSocket.hpp"
#include "CGI.hpp"
#include <unistd.h>

CGI::CGI()
:_read(new Read(this)),
_write(new Write(this))
{
}

CGI::CGI(
	ISubject * subject,
	std::list<ICommand *> * commands,
	const std::string & path
)
:HTTPMethodReceiver(subject, commands),
_read(new Read(this)),
_write(new Write(this)),
_path(path)
{
}


CGI::CGI(
	ISubject * subject,
	std::list<ICommand *> * commands,
	const std::string & path,
	bool is_executable
)
:HTTPMethodReceiver(subject, commands),
_read(new Read(this)),
_write(new Write(this)),
_path(path),
_is_exutetable(is_executable)
{
}

CGI::CGI(const CGI & another)
:HTTPMethodReceiver(another.getSubject(), another.getCommandList()),
_read(new Read(this)),
_write(new Write(this)),
_path(another._path),
_is_exutetable(another._is_exutetable)
{
}

CGI::~CGI()
{
	delete _read;
	delete _write;
}

void CGI::update(int event)
{
	if (event == POLLIN) {
		getCommandList()->push_back(_read);
	} else {
		getCommandList()->push_back(_write);
	}
}

int CGI::read()
{
	char buff[BUFSIZE];
	ssize_t nb = ::read(_in_fd, buff, BUFSIZE);
	if (nb < 0) {
		return -1;
	} else if (nb == 0) {
		// レスポンス作成フェーズ
		getSubject()->unsubscribe(_in_fd, false, this);
		// _as->createResponse(_buff);
		return 0;
	}
	buff[nb] = '\0';
	_buff += buff;
	return 1;
}

int CGI::write()
{
	ssize_t nb = ::write(_out_fd, _buff.c_str(), _buff.size());
	if (nb == -1) {
		return -1;
	}
	if (nb < _nb + _buff.size()) {
		_nb += nb;
		_buff = _buff.substr(0, nb);
		return 1;
	}
	::close(_out_fd);
	getSubject()->unsubscribe(_out_fd, true, this);
	return 0;
}

int CGI::httpGet()
{
	executeCGI(GET);
	getSubject()->subscribe(_in_fd, POLLIN, this);
	return 0;
}

int CGI::httpPost()
{
	executeCGI(POST);
	getSubject()->subscribe(_out_fd, POLLOUT, this);
	getSubject()->subscribe(_in_fd, POLLIN, this);
	return 0;
}

int CGI::httpDelete()
{
	executeCGI(DELETE);
	getSubject()->subscribe(_in_fd, POLLIN, this);
	return 0;
}

static void    perror_and_exit(std::string str) //
{
    std::perror(str.c_str());
    std::exit(1);
}

void CGI::setMetaVariables(HTTPMethod method)
{
	if (method != POST)
	{
		const char *QUERY_STRING_VALUE = "abc=ABC&def=DEF"; //
		// GETの場合はリクエストターゲットから取得、POSTの場合は標準入力から取得
		if (setenv("QUERY_STRING", QUERY_STRING_VALUE, 1) == -1)
			perror_and_exit("set_env");
	}
	//if (/*　message_bodyが存在 */)
	//{
	//	if (setenv("CONTENT_LENGTH", /* message_bodyのlength */, 1) == -1)
	//		perror_and_exit("set_env");
	//}
	if (setenv("PATH_INFO", "/aaa/bbb", 1) == -1)
		perror_and_exit("setenv");

	std::string m;
	if (method == GET) {
		m = "GET";
	} else if (method == POST) {
		m = "POST";
	} else if (method == DELETE) {
		m = "DELETE";
	}
	if (setenv("REQUEST_METHOD", m.c_str(), 1) == -1)
		perror_and_exit("setenv");
}

extern char **environ;

void CGI::executeCGI(HTTPMethod method)
{
	int pipe_fd[2];
    pid_t   pid;
    pid_t   w_pid;
    int status;

	if (pipe(pipe_fd) == -1)
		perror_and_exit("pipe");
	if (method != POST)
	{
		pid = fork();
		if (pid < 0)
			perror_and_exit("fork");
		else if (pid == 0)
		{
			if (close(pipe_fd[0]) == -1)
				perror_and_exit("close");
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				perror_and_exit("dup2");
			// setMetaVariables(method);
			execve("./cgi_scripts/perl.cgi", NULL, environ);
		}
	}
	// else if (method == POST)
	// {
	// 	write(pipe_fd[1], "abc\0", 4); //
	// 	pid = fork();
	// 	if (pid < 0)
	// 		perror_and_exit("fork");
	// 	else if (pid == 0)
	// 	{
	// 		char buf[1000];
	// 		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	// 			perror_and_exit("dup2");
	// 		// ssize_t n = read(STDIN_FILENO, buf, 4);
	// 		// std::cout << buf << std::endl;
	// 		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	// 			perror_and_exit("dup2");
	// 		setMetaVariables(method);
	// 		execve("./cgi_scripts/perl.cgi", NULL, environ);
	// 	}
	// }
	if (close(pipe_fd[1]) == -1)
		perror_and_exit("close");
}

int CGI::getInFd() const
{
	return _in_fd;
}

int CGI::getOutFd() const
{
	return _out_fd;
}

const std::string & CGI::getPath() const {
	return _path;
}

bool CGI::getExectableFlag() const
{
	return _is_exutetable;
}

CGI &CGI::setExectableFlag(bool f)
{
	_is_exutetable = f;
	return *this;
}