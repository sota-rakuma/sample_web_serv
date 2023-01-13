#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
:_max_body_size(8192),
_body_buffersize(1024)
{
}

ServerConfig::ServerConfig(const ServerConfig & another)
:_max_body_size(another._max_body_size),
_body_buffersize(another._body_buffersize)
{
}

ServerConfig::~ServerConfig()
{
}

size_t ServerConfig::getMaxBodySize() const
{
	return _max_body_size;
}

size_t ServerConfig::getBodyBufferSize() const
{
	return _body_buffersize;
}

ServerConfig & ServerConfig::setMaxBodySize(size_t size)
{
	_max_body_size = size;
	return *this;
}

ServerConfig & ServerConfig::setBodyBufferSize(size_t size)
{
	_body_buffersize = size;
	return *this;
}
