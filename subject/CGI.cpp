#include "CGI.hpp"

CGI::CGI()
:_read(new Read(this)),
_write(new Write(this))
{
}


CGI::CGI(
	const std::string & path
)
:_path(path)
{
}

CGI::CGI(const CGI & another)
:_path(another._path)
{
}

CGI::~CGI()
{
}

ICommand* CGI::getHandler(int event) const
{
	// 例外処理
}
