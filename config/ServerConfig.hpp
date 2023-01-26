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
		std::string _path;
		std::map<std::string, bool> _allowed_method;
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
		void setLocationAllowedMethod(const std::map<std::string, bool> &method_map);
		void changeAllowedMethodValue(
			const std::string &,
			bool
		);
		void setLocationPath(
			const std::string &
		);
		void setLocationAlias(const std::string &alias_path);
		void setLocationIndex(const std::string &index_file);
		void setLocationUploadPlace(const std::string &upload_place);
		void setLocationAutoIndex(const std::string &on);
		void setLocationReturn(const int, const std::string &);
		void setLocationExtension(const std::vector<std::string> &);
		// for test
		const std::string & getPath() const;
		const std::string & getAlias() const;
		const std::string & getIndexFile() const;
		const std::string & getUploadPlace() const;
		const std::string & getAutoIndex() const;
		const std::map<std::string, bool> &getAllowedMethod() const;
		const std::vector<std::string> & getCgiExtensions() const;
		const std::pair<int, std::string>& getReturn() const;
	};
private:
	std::string _listen;
	std::string _server_name;
	// size_t _max_body_size;
	int _max_body_size;
	std::map<int, std::string> _default_error_pages;
	// std::map<HTTPStatus, std::string> _default_error_pages;
	std::vector<Location> _locations;
public:
	ServerConfig();
	ServerConfig(const ServerConfig &);
	~ServerConfig();
	// size_t getMaxBodySize() const;
	// ServerConfig & setMaxBodySize(size_t);
	const Location & tryGetLocation(const std::string &) const;
	void setListen(const std::string&);
	void setServerName(const std::string&);
	void setMaxBodySize(const int);
	void setDefaultErrorPages(
		const std::vector<int> &,
		const std::string &
	);
	void setAllowedMethod(
		size_t,
		const std::map<std::string, bool> &
	);
	void setAllowedMethod(
		size_t,
		const std::string &,
		bool
	);
	void setIndex(size_t, const std::string &);
	void setAlias(size_t, const std::string &);
	void setUploadPlace(size_t, const std::string&);
	void setAutoIndex(size_t, const std::string &);
	void setReturn(size_t, const int, const std::string&);
	void setExtension(size_t, const std::vector<std::string> &);
	void setPath(
		size_t,
		const std::string &
	);
	void addLocation();
	 // for test
	const std::string &getListen() const;
	const std::string &getServerName() const;
	int getMaxBodySize() const;
	const std::map<int, std::string> &getDefaultErrorPage() const;
	const std::vector<Location> &getLocationVec() const;
};

#endif /* SERVERCONFIG_HPP */
