#ifndef SERVERCONFIGCONTROLLER_HPP
#define SERVERCONFIGCONTROLLER_HPP

#include "ServerConfig.hpp"
#include <stdexcept>
#include <vector>
#include <map>

class ServerConfigController
{
public:
	class FileError : std::runtime_error
	{
	public:
		FileError();
		FileError(const std::string &);
		~FileError();
	};
	/*
	ServerConfigParser _p;
	setServerConfig(const SyntaxTree &);
	↑default_serverの設定も必須
	*/
private:
	std::vector<ServerConfig> _infos;
	std::map<std::string, size_t> _keys;
public:
	ServerConfigController();
	ServerConfigController(const std::string &);
	ServerConfigController(
		const ServerConfigController &
	);
	~ServerConfigController();
	const std::vector<ServerConfig> & getConfigVec() const;
	const std::map<std::string, size_t> getKeyMap() const;
	const ServerConfig & getConfig(const std::string &) const;
};

#endif /* SERVERCONFIGCONTROLLER_HPP */
