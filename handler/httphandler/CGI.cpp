#include "../AcceptedSocket.hpp"
#include "CGI.hpp"
#include <unistd.h>

#ifndef IN
#define IN 0
#endif

#ifndef OUT
#define OUT 1
#endif


CGI::CGI()
:_buff(""),
_read(new Read(this)),
_write(new Write(this))
{
}

CGI::CGI(
	ISubject * subject,
	std::list<ICommand *> * commands,
	const std::string & path,
	AcceptedSocket * as
)
:HTTPMethodReceiver(subject, commands),
_buff(""),
_read(new Read(this)),
_write(new Write(this)),
_path(path),
_as(as)
{
}


CGI::CGI(
	ISubject * subject,
	std::list<ICommand *> * commands,
	const std::string & path,
	bool is_executable,
	AcceptedSocket *as
)
:HTTPMethodReceiver(subject, commands),
_buff(""),
_read(new Read(this)),
_write(new Write(this)),
_path(path),
_is_exutetable(is_executable),
_as(as)
{
}

CGI::CGI(const CGI & another)
:HTTPMethodReceiver(another.getSubject(), another.getCommandList()),
_buff(another._buff),
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
	if (event & (POLLNVAL | POLLERR)) {
		if (event == POLLNVAL)
		{
			std::cout << "POLLNVAL" << std::endl;
		} else {
			std::cout << "POLLERR" << std::endl;
		}
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], false);
	}
	if (event & POLLHUP) {
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], false);
		_as->processCGIResponse(_buff);
	} else if (event & POLLOUT) {
		getCommandList()->push_back(_write);
	} else if (event & POLLIN) {
		std::cout << "read " << std::endl;
		getCommandList()->push_back(_read);
	}
}

int CGI::read()
{
	char buff[BUFSIZE];
	ssize_t nb = ::read(_c_to_p[IN], buff, BUFSIZE);
	if (nb < 0) {
		// error レスポンス
		::close(_p_to_c[OUT]);
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], false);
		return -1;
	} else if (nb == 0) {
		getSubject()->unsubscribe(_p_to_c[OUT], true);
		getSubject()->unsubscribe(_c_to_p[IN], false);
		// ::close(_pipe_fd[0]);
		// レスポンス作成フェーズ
		_as->processCGIResponse(_buff);
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
		// error レスポンス
		::close(_p_to_c[OUT]);
		getSubject()->unsubscribe(_c_to_p[IN], false);
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
	executeCGI(GET);
	getSubject()->subscribe(_c_to_p[IN], POLLIN, this);
	return 0;
}

int CGI::httpPost()
{
	executeCGI(POST);
	_buff = "value=aaaa&value_2=bbbb";
	getSubject()->subscribe(_p_to_c[OUT], POLLOUT, this);
	getSubject()->subscribe(_c_to_p[IN], POLLIN, this);
	return 0;
}

int CGI::httpDelete()
{
	executeCGI(DELETE);
	getSubject()->subscribe(_c_to_p[IN], POLLIN, this);
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
			perror_and_exit("setenv");
	}
	//if (/*　message_bodyが存在 */)
	//{
	//	if (setenv("CONTENT_LENGTH", /* message_bodyのlength */, 1) == -1)
	//		perror_and_exit("setenv");
		// if (setenv("CONTENT_TYPE", "", 1) == -1) // html/textとか 判別方法どうする？？
		// 	perror_and_exit("setenv");
	//}
	if (setenv("PATH_INFO", "/aaa/bbb", 1) == -1)
		perror_and_exit("setenv");
	if (setenv("AUTH_TYPE", "", 1) == -1) //
		perror_and_exit("setenv");
	if (setenv("GATEWAY_INTERFACE", "CGI/1.1", 1) == -1) //
		perror_and_exit("setenv");
	if (setenv("REMOTE_ADDR", "", 1) == -1) // クライアントのIPアドレスを設定する
		perror_and_exit("setenv");
	if (setenv("SCRIPT_NAME", "", 1) == -1) // CGIスクリプトのpath(URI)を設定する SCRIPT_NAMEの値にはPATH_INFO部分を含めない
		perror_and_exit("setenv");
	if (setenv("SERVER_NAME", "", 1) == -1) // サーバーの名前　configファイルのserver_nameディレクティブを使う？
		perror_and_exit("setenv");
	if (setenv("SERVER_PORT", "80", 1) == -1) // サーバーのポートを設定
		perror_and_exit("setenv");
	if (setenv("SERVER_PROTOCOL", "HTTP/1.1", 1) == -1) //
		perror_and_exit("setenv");
	if (setenv("SERVER_SOFTWARE", "Debian", 1) == -1) // CGI プログラムを起動した Web サーバソフトウエアの名前 例:Apacheとか
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
	//int pipe_fd[2];
    pid_t   pid;
    pid_t   w_pid;
    int status;

	// if (pipe(_pipe_fd) == -1)
	// 	perror_and_exit("pipe");
	if (pipe(_p_to_c) == -1)
		perror_and_exit("pipe");
	if (pipe(_c_to_p) == -1)
		perror_and_exit("pipe");
	if (method != POST)
	{
		pid = fork();
		if (pid < 0)
			perror_and_exit("fork");
		else if (pid == 0)
		{
			if (close(_c_to_p[IN]) == -1)
				perror_and_exit("close");
			if (dup2(_p_to_c[OUT], STDOUT_FILENO) == -1)
				perror_and_exit("dup2");
			// setMetaVariables(method);
			if (execve(_path.c_str(), NULL, environ) == -1) {
				perror("execve");
			}
		}
		if (close(_c_to_p[OUT]) == -1)
			perror_and_exit("close");
	}
	 else if (method == POST)
	 {
	 	//::write(_pipe_fd[1], "abc\0", 4);
	 	pid = fork();
	 	if (pid < 0)
	 		perror_and_exit("fork");
	 	else if (pid == 0)
	 	{
			close(_c_to_p[IN]);
			close(_p_to_c[OUT]);
	 		if (dup2(_p_to_c[IN], STDIN_FILENO) == -1)
	 			perror_and_exit("dup2");

	 		if (dup2(_c_to_p[OUT], STDOUT_FILENO) == -1)
	 			perror_and_exit("dup2");

	 		//setMetaVariables(method);
	 		execve(_path.c_str(), NULL, environ);
	 	}
		close(_c_to_p[OUT]);
		close(_p_to_c[IN]);
	 }
}

int CGI::getInFd() const
{
	return _c_to_p[IN];
}

int CGI::getOutFd() const
{
	return _p_to_c[OUT];
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