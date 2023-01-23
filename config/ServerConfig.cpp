#include "ServerConfig.hpp"
#include <stdexcept>

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
:_alias("default"), _index_file("default"), _upload_place("default"), _autoindex("off"),
_cgi_extensions()
{
	_allowed_method["GET"] = false;
	_allowed_method["POST"] = false;
	_allowed_method["DELETE"] = false;
	_return = std::make_pair(0, "default");
}

ServerConfig::Location::Location(const ServerConfig::Location & another)
:_alias(another._alias), _index_file(another._index_file), _upload_place(another._upload_place), _autoindex(another._autoindex),
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

const ServerConfig::Location & ServerConfig::tryGetLocation(
	const std::string & path
)
{
	std::map<std::string, Location>::iterator candidate = _locations.end();
	for (std::map<std::string, Location>::iterator it = _locations.begin();
		it != _locations.end();
		it++)
	{
		if (path.rfind(it->first, it->first.size() - 1) != std::string::npos)
		{
			candidate = it;
		}
	}
	if (candidate == _locations.end()) {
		throw std::runtime_error("this resource is forbidden");
	}
	return candidate->second;
}

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

void ServerConfig::setAllowedMethod(const std::string path, const std::map<std::string, bool> method_map) {
	// for (size_t i = 0; i < method_vec.size(); i++) {
		// _locations[path]._allowed_method[i] = method_vec[i]; //= method_vec[i];
		_locations[path].setLocationAllowedMethod(method_map); //= method_vec[i];
	// }
}

void ServerConfig::setAllowedMethod(
	const std::string & path,
	const std::string & key,
	bool val
)
{
	_locations[path].changeAllowedMethodValue(key, val);
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

void ServerConfig::setAutoIndex(const std::string path, const std::string on) {
	_locations[path].setLocationAutoIndex(on);
}

void ServerConfig::setReturn(const std::string path, const int status_code, const std::string return_uri) {
	_locations[path].setLocationReturn(status_code, return_uri);
}

void ServerConfig::setExtension(const std::string path, const std::vector<std::string> extension_vec) {
	_locations[path].setLocationExtension(extension_vec);
}

void ServerConfig::Location::setLocationAllowedMethod(const std::map<std::string, bool> method_map) {
	for (std::map<std::string, bool>::const_iterator it = method_map.begin();
		it != method_map.end();
		it++)
	{
		_allowed_method[it->first] = it->second;
	}
}

void ServerConfig::Location::changeAllowedMethodValue(
	const std::string & key,
	bool val
)
{
	_allowed_method[key] = val;
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

void ServerConfig::Location::setLocationAutoIndex(const std::string on) {
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