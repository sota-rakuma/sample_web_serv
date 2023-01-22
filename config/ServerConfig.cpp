#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
:_listen(""), _server_name("default"), _max_body_size(8192)
{
	_default_error_pages[0] = "default";
}

ServerConfig::ServerConfig(const ServerConfig & another)
:_max_body_size(another._max_body_size), _listen(another._listen), _server_name(another._server_name), \
_default_error_pages(another._default_error_pages), _locations(another._locations)
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::Location::Location()
:_alias("default"), _index_file("default"), _upload_place("default"), _autoindex(false)
{
	_allowed_method.push_back("default");
	_return = std::make_pair(0, "default");
	_cgi_extensions.push_back("default");
}

ServerConfig::Location::Location(const ServerConfig::Location & another)
:_alias(another._alias), _index_file(another._index_file), _upload_place(another._upload_place), _autoindex(another._autoindex), \
_allowed_method(another._allowed_method), _return(another._return), _cgi_extensions(another._cgi_extensions)
{
}

ServerConfig::Location::~Location() {
}

// size_t ServerConfig::getMaxBodySize() const
// {
// 	return _max_body_size;
// }

// ServerConfig & ServerConfig::setMaxBodySize(size_t size)
// {
// 	_max_body_size = size;
// 	return *this;
// }

// const ServerConfig::Location & getLocation(
// 	const std::string & url
// )
// {
// }

void ServerConfig::setListen(const std::string value) {
	_listen = value;
}

void ServerConfig::setServerName(const std::string value) {
	_server_name = value;
}

void ServerConfig::setMaxBodySize(const int value) {
	_max_body_size = value;
}

void ServerConfig::setDefaultErrorPages(const std::vector<int> status_codes, const std::string dep) {
	for (size_t i = 0; i < status_codes.size(); i++)
		_default_error_pages.insert(std::make_pair(status_codes[i], dep));
}

void ServerConfig::setAllowedMethod(const std::string path, const std::vector<std::string> method_vec) {
	for (size_t i = 0; i < method_vec.size(); i++) {
		// _locations[path]._allowed_method[i] = method_vec[i]; //= method_vec[i];
		_locations[path].setLocationAllowedMethod(method_vec); //= method_vec[i];
	}
}

void ServerConfig::setIndex(const std::string path, const std::string index_file) {
	_locations[path].setLocationIndex(index_file);
}

void ServerConfig::setAlias(const std::string path, const std::string alias_path) {
	// std::cout << "alias_path in setAlias = " << alias_path << std::endl;
	_locations[path].setLocationAlias(alias_path);
}

void ServerConfig::setUploadPlace(const std::string path, const std::string upload_place) {
	_locations[path].setLocationUploadPlace(upload_place);
}

void ServerConfig::setAutoIndex(const std::string path, const bool on) {
	_locations[path].setLocationAutoIndex(on);
}

void ServerConfig::setReturn(const std::string path, const int status_code, const std::string return_uri) {
	_locations[path].setLocationReturn(status_code, return_uri);
}

void ServerConfig::setExtension(const std::string path, const std::vector<std::string> extension_vec) {
	_locations[path].setLocationExtension(extension_vec);
}

void ServerConfig::Location::setLocationAllowedMethod(const std::vector<std::string> method_vec) {
	for (size_t i = 0; i < method_vec.size(); i++) {
		_allowed_method.push_back(method_vec[i]);
	}
}

void ServerConfig::Location::setLocationIndex(const std::string index_file) {
	_index_file = index_file;
}

void ServerConfig::Location::setLocationAlias(const std::string alias_path) {
	_alias = alias_path;
}

void ServerConfig::Location::setLocationUploadPlace(const std::string upload_place) {
	_upload_place = upload_place;
}

void ServerConfig::Location::setLocationAutoIndex(const bool on) {
	_autoindex = on;
}

void ServerConfig::Location::setLocationReturn(const int status_code, const std::string return_uri) {
	_return.first = status_code;
	_return.second = return_uri;
}

void ServerConfig::Location::setLocationExtension(const std::vector<std::string> extension_vec) {
	for (size_t i = 0; i < extension_vec.size(); i++) {
		_cgi_extensions.push_back(extension_vec[i]);
	}
}