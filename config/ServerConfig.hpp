#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>

class ServerConfig
{
private:
	std::string _node; // IPアドレス
	std::string _service; // ポート番号
/*
	std::vector<>
*/
public:
	ServerConfig();
	ServerConfig(const ServerConfig &);
	~ServerConfig();
	const std::string & getNode() const;
	const std::string & getService() const;
};

#endif /* SERVERCONFIG_HPP */
