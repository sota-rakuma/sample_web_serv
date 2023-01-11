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
public:
	ServerConfig();
	ServerConfig(const ServerConfig &);
	~ServerConfig();
	size_t getMaxBodySize() const;
};

#endif /* SERVERCONFIG_HPP */
