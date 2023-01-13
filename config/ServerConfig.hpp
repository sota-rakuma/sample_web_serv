#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>

class ServerConfig
{
private:
/*
	location
*/
	size_t _max_body_size;
	size_t _body_buffersize;
public:
	ServerConfig();
	ServerConfig(const ServerConfig &);
	~ServerConfig();
	size_t getMaxBodySize() const;
	size_t getBodyBufferSize() const;
	ServerConfig & setMaxBodySize(size_t);
	ServerConfig & setBodyBufferSize(size_t);
};

#endif /* SERVERCONFIG_HPP */
