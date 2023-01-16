#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
:_max_body_size(8192)
{
}

ServerConfig::ServerConfig(const ServerConfig & another)
:_max_body_size(another._max_body_size)
{
}

ServerConfig::~ServerConfig()
{
}

size_t ServerConfig::getMaxBodySize() const
{
	return _max_body_size;
}

ServerConfig & ServerConfig::setMaxBodySize(size_t size)
{
	_max_body_size = size;
	return *this;
}

const ServerConfig::Location & getLocation(
	const std::string & url
)
{
}