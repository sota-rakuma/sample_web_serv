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
#include "../config/ServerConfig.hpp"
#include "TargetParser.hpp"
#include "Parser.hpp"

class ConfigParser
{
	private:
		std::vector<ServerConfig> _sc_vec;
		size_t _server_i;
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
		int parseArrangedLocationDirectives(
			size_t
		);
		int parseAllowedMethod(
			size_t
		);
		int parseIndex(size_t);
		int parseAlias(size_t);
		int parseAutoIndex(size_t);
		int parseReturn(size_t);
		int parseUploadPlace(size_t);
		int parseExtension(size_t);
		void initDirectiveFlag();
		void initLocationDirectiveFlag();
		const std::vector<ServerConfig> &
		getScVec() const;
		size_t getScVecSize() const;
		const std::string &getListen(size_t i) const;
		const std::string & getServerName(size_t i) const;
		int getMaxBodySize(size_t i) const;
		const std::map<int, std::string> &
		getDefaultErrorPage(size_t i) const ;

		const std::vector<ServerConfig::Location> &getLocationVec(size_t i) const;
		const ServerConfig::Location & getLocation
		(
			size_t index,
			const std::string & path
		) const;
};

#endif /* CONFIGPARSER_HPP */