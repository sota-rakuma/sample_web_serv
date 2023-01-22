#ifndef SERVERCONFIGFINDER_HPP
#define SERVERCONFIGFINDER_HPP

#include "ServerConfig.hpp"
#include <stdexcept>
#include <vector>
#include <map>

// listen socketに入れるのを想定
class ServerConfigFinder
{
public:
	class FileError : std::runtime_error
	{
	public:
		FileError();
		FileError(const std::string &);
		virtual ~FileError() throw();
	};
	/*
	ServerConfigParser _p;
	setServerConfig(const SyntaxTree &);
	↑default_serverの設定も必須
	*/
private:
	std::vector<ServerConfig> _infos;
	// server-name : ↑のindex
	std::map<std::string, size_t> _keys;
public:
	ServerConfigFinder();
	ServerConfigFinder(const std::string &);
	ServerConfigFinder(
		const ServerConfigFinder &
	);
	~ServerConfigFinder();
	const std::vector<ServerConfig> & getConfigVec() const;
	const std::map<std::string, size_t> getKeyMap() const;
	const ServerConfig & getConfig(const std::string &) const;
	// void setFactor(const size_t, const std::string);
};

#endif /*SERVERCONFIGFINDER_HPP*/
