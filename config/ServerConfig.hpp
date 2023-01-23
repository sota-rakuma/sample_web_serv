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
		std::vector<std::string> _allowed_method;
		std::string _alias;
		std::string _index_file;
		std::string _upload_place;
		std::string _autoindex;
		std::pair<int, std::string> _return;
		std::vector<std::string> _cgi_extensions;
	public:
		Location();
		Location(const ServerConfig::Location & another);
		~Location();
		void setLocationAllowedMethod(const std::vector<std::string> method_vec);
		void setLocationAlias(const std::string alias_path);
		void setLocationIndex(const std::string index_file);
		void setLocationUploadPlace(const std::string upload_place);
		void setLocationAutoIndex(const std::string on);
		void setLocationReturn(const int, const std::string);
		void setLocationExtension(const std::vector<std::string>);
		// for test
		std::string getAlias() const {return _alias;};
		std::string getIndexFile() const {return _index_file;};
		std::string getUploadPlace() const {return _upload_place;};
		std::string getAutoIndex() const {return _autoindex;};
		std::vector<std::string> getAllowedMethod() const {return _allowed_method;};
		std::vector<std::string> getCgiExtensions() const {return _cgi_extensions;};
		std::pair<int, std::string> getReturn() const {return _return;};
	};
private:
	std::string _listen;
	std::string _server_name;
	// size_t _max_body_size;
	int _max_body_size;
	std::map<int, std::string> _default_error_pages;
	// std::map<HTTPStatus, std::string> _default_error_pages;
	std::map<std::string, Location> _locations;
public:
	ServerConfig();
	ServerConfig(const ServerConfig &);
	~ServerConfig();
	// size_t getMaxBodySize() const;
	// ServerConfig & setMaxBodySize(size_t);
	// const Location & getLocation(const std::string &);
	void setListen(const std::string);
	void setServerName(const std::string);
	void setMaxBodySize(const int);
	void setDefaultErrorPages(const std::vector<int>, const std::string);
	void setAllowedMethod(const std::string, const std::vector<std::string>);
	void setIndex(const std::string, const std::string);
	void setAlias(const std::string, const std::string);
	void setUploadPlace(const std::string, const std::string);
	void setAutoIndex(const std::string, const std::string);
	void setReturn(const std::string, const int, const std::string);
	void setExtension(const std::string, const std::vector<std::string>);
	 // for test
	std::string getListen() const {return _listen;};
	std::string getServerName() const {return _server_name;};
	int getMaxBodySize() const {return _max_body_size;};
	std::map<int, std::string> getDefaultErrorPage() const {return _default_error_pages;};
	std::map<std::string, Location> getLocationMap() const {return _locations;};
};

#endif /* SERVERCONFIG_HPP */
