#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "../HTTP/HTTPRequest.hpp"
#include <string>
#include <vector>
#include <map>

class ServerConfig
{
public:
	class Location {
	private:
		int _accepted_method;// HTTPMethodをintに変換
		std::string _alias;
		std::string _index_file;
		std::string _upload_place;
		bool _autoindex;
		std::vector<std::string> _cgi_extentions;
	};
private:
	size_t _max_body_size;
	std::map<HTTPStatus, std::string> _default_error_pages;
	std::map<std::string, Location> _locations;
public:
	ServerConfig();
	ServerConfig(const ServerConfig &);
	~ServerConfig();
	size_t getMaxBodySize() const;
	ServerConfig & setMaxBodySize(size_t);
	const Location & getLocation(const std::string &);
};

#endif /* SERVERCONFIG_HPP */
