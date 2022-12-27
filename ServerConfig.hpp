#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>

class ServerConfig
{
private:
	std::string _node; // IPアドレス
	std::string _service; // ポート番号
public:
	ServerConfig();
	ServerConfig(const ServerConfig &);
	~ServerConfig();
	const std::string & getNode() const;
	const std::string & getService() const;
};

#endif /* SERVERCONFIG_HPP */
