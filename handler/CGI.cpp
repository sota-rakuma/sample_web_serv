#include "CGI.hpp"
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
}

int CGI::httpPost()
{
	if (_is_exist == false) {
		executeCGI(OUT);
		_is_exist = true;
		return 1;
	}
	ssize_t nb = write(_out_fd, _buff.c_str(), _buff.size());
	if (nb < _nb + _buff.size()) {
		_nb += nb;
		return 1;
	}
	return httpGet();
}

void CGI::executeCGI(int event)
{
}