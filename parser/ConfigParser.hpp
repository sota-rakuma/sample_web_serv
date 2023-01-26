#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <unistd.h>
#include <set>
#include <map>
#include "../config/ServerConfigFinder.hpp"
#include "../config/ServerConfig.hpp"
#include "TargetParser.hpp"
#include "Parser.hpp"

class ConfigParser
{
	private:
		std::vector<ServerConfig> _sc_vec;
		size_t _server_i;
		// ServerConfigFinder _scf;
		std::string _raw;
		size_t  _i;
		std::vector<std::string> _searched_directives;
			// {"\tlisten ", "\tserver_name ", "\tmax_body_size ", "\terror_page ", "\n\tlocation "};
		std::vector<std::string> _searched_location_directives;
			// {"\t\tallowed_method ", "\t\tindex ", "\t\talias ", "\t\tautoindex ", "\t\treturn ", "\t\tupload_place ", "\t\textension "};
		std::vector<std::string> _allowed_methods;
			// {"GET", "POST", "DELETE"};
		bool _listen_flag;
		bool _server_name_flag;
		bool _max_body_size_flag;
		bool _location_allowed_method_flag;
		bool _location_alias_flag;
		bool _location_index_flag;
		bool _location_upload_place_flag;
		bool _location_autoindex_flag;
		bool _location_return_flag;
		bool _location_extension_flag;
	public:
		ConfigParser();
		virtual ~ConfigParser();
		virtual int parse(const std::string &);
		int checkPermission(const std::string &file);
		int parseServer();
		int parseArrangedDirectives();
		int parseListen();
		int parseServerName();
		int parseMaxBodySize();
		int parseErrorPage();
		int parseLocation();
		int parseArrangedLocationDirectives(const std::string &);
		int parseAllowedMethod(std::string);
		int parseIndex(std::string);
		int parseAlias(std::string);
		int parseAutoIndex(std::string);
		int parseReturn(std::string);
		int parseUploadPlace(std::string);
		int parseExtension(std::string);
		void initDirectiveFlag();
		void initLocationDirectiveFlag();
		const std::vector<ServerConfig> &
		getScVec() const;
		size_t getScVecSize() const {return _sc_vec.size();};
		std::string getListen(size_t i) const {return _sc_vec[i].getListen();};
		std::string getServerName(size_t i) const {return _sc_vec[i].getServerName();};
		int getMaxBodySize(size_t i) const {return _sc_vec[i].getMaxBodySize();};

		const std::map<int, std::string> & const
		getDefaultErrorPage(size_t i) {return _sc_vec[i].getDefaultErrorPage();};

		const std::vector<ServerConfig::Location> &getLocationVec(size_t i) {return _sc_vec[i].getLocationVec();};
		const ServerConfig::Location & getLocation
		(
			size_t index,
			const std::string & path
		) const
		{
			return _sc_vec[index].tryGetLocation(path);
		}
};

#endif /* CONFIGPARSER_HPP */