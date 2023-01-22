#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <unistd.h>
#include "../config/ServerConfigFinder.hpp"
#include "../config/ServerConfig.hpp"

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
            // {"\t\tallowed_method ", "\t\tindex ", "\t\talias ", "\t\tautoindex on", "\t\treturn ", "\t\tupload_place ", "\t\textension "};
        std::vector<std::string> _allowed_methods;
            // {"GET", "POST", "DELETE"};
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
        int parseArrangedLocationDirectives(std::string);
        int parseAllowedMethod(std::string);
        int parseIndex(std::string);
        int parseAlias(std::string);
        int parseAutoIndex(std::string);
        int parseReturn(std::string);
        int parseUploadPlace(std::string);
        int parseExtension(std::string);
        // for test
        size_t getScVecSize() const {return _sc_vec.size();};
        std::string getListen(size_t i) const {return _sc_vec[i].getListen();};
        std::string getServerName(size_t i) const {return _sc_vec[i].getServerName();};
        int getMaxBodySize(size_t i) const {return _sc_vec[i].getMaxBodySize();};
        std::map<int, std::string> getDefaultErrorPage(size_t i) const {return _sc_vec[i].getDefaultErrorPage();};
        std::map<std::string, ServerConfig::Location> getLocation(size_t i) const {return _sc_vec[i].getLocation();};
};

#endif /* CONFIGPARSER_HPP */