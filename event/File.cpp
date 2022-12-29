#include "File.hpp"

File::File()
:_buff()
{
}

File::File(
	const std::string & buff
)
:_buff(buff)
{
}

File::File(
	const std::string & buff
)
:_buff(buff)
{
}

File::File(
	const File & another
)
:_buff(another._buff)
{
}

File::~File()
{
}



EventHandler::Status File::specialRead(int fd)
{
}

EventHandler::Status File::specialWrite(int fd)
{
}



const std::string &File::getBuff() const
{
	return _buff;
}

const std::string &File::setBuff(
	const std::string & buff
)
{
	_buff = buff;
}