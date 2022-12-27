#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
}

ServerConfig::ServerConfig(const ServerConfig & another)
{
}

ServerConfig::~ServerConfig()
{
}

const std::string & ServerConfig::getNode() const
{
	return _node;
}

const std::string & ServerConfig::getService() const
{
	return _service;
}