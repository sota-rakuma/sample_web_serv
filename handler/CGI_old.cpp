#include "CGI_old.hpp"
#include <unistd.h>

CGI::CGI()
:_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this))
{
}


CGI::CGI(
	const std::string & path
)
:_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this)),
_is_exist(false),
_path(path)
{
}

CGI::CGI(const CGI & another)
:_get(new Get(this)),
_post(new Post(this)),
_delete(new Delete(this)),
_is_exist(false),
_path(another._path)
{
}

CGI::~CGI()
{
	delete _get;
	delete _post;
	delete _delete;
}

static void    perror_and_exit(std::string str) //
{
    std::perror(str.c_str());
    std::exit(1);
}

int CGI::httpDelete()
{
	executeCGI(DELETE);
}

int CGI::httpPost()
{
	executeCGI(POST);
	// if (_is_exist == false) {
	// 	executeCGI(OUT);
	// 	_is_exist = true;
	// 	return 1;
	// }
	// ssize_t nb = write(_out_fd, _buff.c_str(), _buff.size());
	// if (nb < _nb + _buff.size()) {
	// 	_nb += nb;
	// 	return 1;
	// }
	// return httpGet();
}

int CGI::httpGet()
{
	executeCGI(GET);
}

void CGI::setMetaVariables(std::string method)
{
	if (method != POST)
	{
		char *QUERY_STRING_VALUE = "abc=ABC&def=DEF"; //
		// GETの場合はリクエストターゲットから取得、POSTの場合は標準入力から取得
		if (setenv("QUERY_STRING", QUERY_STRING_VALUE, 1) == -1)
			perror_and_exit("set_env");
	}
	if (/*　message_bodyが存在 */)
	{
		if (setenv("CONTENT_LENGTH", /* message_bodyのlength */, 1) == -1)
			perror_and_exit("set_env");
	}
	if (setenv("PATH_INFO", "/aaa/bbb", 1) == -1)
		perror_and_exit("setenv");
	if (setenv("REQUEST_METHOD", method.c_str(), 1) == -1)
		perror_and_exit("setenv");
}

extern char **environ; // 正しい場所に設置し直す!!

// void CGI::executeCGI(int event, int method)
void CGI::executeCGI(std::string method)
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
			setMetaVariables(method);
			execve("./cgi_scripts/perl.cgi", NULL, environ);
		}
	}
	else if (method == POST)
	{
		write(pipe_fd[1], "abc\0", 4); //
		pid = fork();
		if (pid < 0)
			perror_and_exit("fork");
		else if (pid == 0)
		{
			char buf[1000];
			if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
				perror_and_exit("dup2");
			// ssize_t n = read(STDIN_FILENO, buf, 4);
			// std::cout << buf << std::endl;
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				perror_and_exit("dup2");
			setMetaVariables(method);
			execve("./cgi_scripts/perl.cgi", NULL, environ);
		}
	}
	if (close(pipe_fd[1]) == -1)
		perror_and_exit("close");
	// w_pid = waitpid(pid, &status, 0); // waitpid必要ない
	// if (WIFEXITED(status) != 1)
	// {
	// 	std::exit(1);
	// }
	char buf[1000];
	if (read(pipe_fd[0], buf, 1000) == -1)
		perror_and_exit("read");
	std::cout << buf << std::endl;
}