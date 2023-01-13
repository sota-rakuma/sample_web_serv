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
	if (event == IN) {
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
		return 1;
	}
	return 0;
}

int CGI::httpPost()
{
	executeCGI(OUT);
	getSubject()->subscribe(_out_fd, OUT, this);
	getSubject()->subscribe(_in_fd, IN, this);
	return 0;
}

void CGI::executeCGI(int event)
{
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