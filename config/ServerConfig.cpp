#include "ServerConfig.hpp"
#include <stdexcept>
#include "../utils/utils.hpp"

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
:_alias(), _index_file(), _upload_place(), _autoindex(false),
_cgi_extensions()
{
	_allowed_method["GET"] = false;
	_allowed_method["POST"] = false;
	_allowed_method["DELETE"] = false;
	_return = std::make_pair(0, "default");
}

ServerConfig::Location::Location(const ServerConfig::Location & another)
:_path(another._path),
_alias(another._alias),
_index_file(another._index_file),
_upload_place(another._upload_place),
_autoindex(another._autoindex),
_allowed_method(another._allowed_method),
_return(another._return),
_cgi_extensions(another._cgi_extensions)
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
) const
{
	size_t slash = path.rfind('/');
	if (slash == std::string::npos) {
		throw std::runtime_error("this resource is forbidden");
	}
	size_t candidate = _locations.size();
	for (size_t i = 0; i < _locations.size(); i++)
	{
		if (slash < _locations[i].getPath().size() - 1) {
			continue;
		}
		if (path.rfind(_locations[i].getPath(),
					_locations[i].getPath().size() - 1) != std::string::npos)
		{
			candidate = i;
		}
	}
	if (candidate == _locations.size()) {
		throw std::runtime_error("this resource is forbidden");
	}
	return _locations[candidate];
}

void ServerConfig::setListen(const std::string &value) {
	_listen = value;
}

void ServerConfig::setServerName(const std::string &value) {
	_server_name = value;
}

void ServerConfig::setMaxBodySize(const int value) {
	_max_body_size = value;
}

void ServerConfig::setDefaultErrorPages(
	const std::vector<int> &status_codes,
	const std::string &dep
)
{
	for (size_t i = 0; i < status_codes.size(); i++) {
		_default_error_pages.insert(std::make_pair(status_codes[i], dep));
	}
}

void ServerConfig::eraseDefaultErrorPage(int status)
{
	_default_error_pages.erase(status);
}

void ServerConfig::setAllowedMethod(
	size_t index,
	const std::map<std::string, bool> &method_map
)
{
		_locations[index].setLocationAllowedMethod(method_map);
}

void ServerConfig::setAllowedMethod(
	size_t index,
	const std::string & key,
	bool val
)
{
	_locations[index].changeAllowedMethodValue(key, val);
}

void ServerConfig::setIndex(
	size_t index,
	const std::string &index_file
) {
	_locations[index].setLocationIndex(index_file);
}

void ServerConfig::setAlias(
	size_t index,
	const std::string &alias_path
) {
	// std::cout << "alias_path in setAlias = " << alias_path << std::endl;
	_locations[index].setLocationAlias(alias_path);
}

void ServerConfig::setUploadPlace(
	size_t index,
	const std::string &upload_place
) {
	_locations[index].setLocationUploadPlace(upload_place);
}

void ServerConfig::setAutoIndex(
	size_t index,
	bool on
) {
	_locations[index].setLocationAutoIndex(on);
}

void ServerConfig::setReturn(
	size_t index,
	const int status_code,
	const std::string &return_uri
) {
	_locations[index].setLocationReturn(status_code, return_uri);
}

void ServerConfig::setExtension(
	size_t index,
	const std::vector<std::string> &extension_vec
) {
	_locations[index].setLocationExtension(extension_vec);
}

void ServerConfig::setPath(
	size_t index,
	const std::string &path
)
{
	_locations[index].setLocationPath(path);
}

void ServerConfig::addLocation()
{
	_locations.push_back(Location());
}

void ServerConfig::Location::setLocationAllowedMethod(const std::map<std::string, bool> &method_map) {
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

void ServerConfig::Location::setLocationPath(
	const std::string & path
)
{
	_path = path;
}

void ServerConfig::Location::setLocationIndex(
	const std::string &index_file
) {
	_index_file = index_file;
}

void ServerConfig::Location::setLocationAlias(
	const std::string &alias_path
) {
	_alias = alias_path;
}

void ServerConfig::Location::setLocationUploadPlace(
	const std::string &upload_place
) {
	_upload_place = upload_place;
}

void ServerConfig::Location::setLocationAutoIndex(
	bool on
) {
	_autoindex = on;
}

void ServerConfig::Location::setLocationReturn(
	const int status_code,
	const std::string &return_uri
) {
	_return.first = status_code;
	_return.second = return_uri;
}

void ServerConfig::Location::setLocationExtension(
	const std::vector<std::string> &extension_vec
) {
	for (size_t i = 0; i < extension_vec.size(); i++) {
		_cgi_extensions.push_back(extension_vec[i]);
	}
}

const std::string & ServerConfig::Location::getPath() const
{
	return _path;
}

const std::string & ServerConfig::Location::getAlias() const {
	return _alias;
}

const std::string & ServerConfig::Location::getIndexFile() const
{
	return _index_file;
};

const std::string & ServerConfig::Location::getUploadPlace() const
{
	return _upload_place;
};

bool ServerConfig::Location::getAutoIndex() const
{
	return _autoindex;
};

const std::map<std::string, bool> &ServerConfig::Location::getAllowedMethod() const
{
	return _allowed_method;
};

const std::vector<std::string> & ServerConfig::Location::getCgiExtensions() const
{
	return _cgi_extensions;
};

const std::pair<int, std::string>& ServerConfig::Location::getReturn() const
{
	return _return;
};

const std::string &ServerConfig::getListen() const
{
	return _listen;
};

const std::string &ServerConfig::getServerName() const
{
	return _server_name;
};

int ServerConfig::getMaxBodySize() const
{
	return _max_body_size;
};

const std::map<int, std::string> &ServerConfig::getDefaultErrorPage() const
{
	return _default_error_pages;
};

const std::vector<ServerConfig::Location> &
 ServerConfig::getLocationVec() const
{
	return _locations;
};


std::ostream & operator<<(std::ostream & os, const ServerConfig &rhs)
{
	os << addColorText("ServerConfig", BLUE) << std::endl
	<< "listen: " << rhs.getListen() << std::endl
	<< "server_name: " << rhs.getServerName() << std::endl
	<< "max body size: " << rhs.getMaxBodySize() << std::endl
	<< "## Defautl Error Pages ##" << std::endl;

	for (std::map<int, std::string>::const_iterator it = rhs.getDefaultErrorPage().begin();
		it != rhs.getDefaultErrorPage().end();
		it++)
	{
		os << "Status: " << it->first << ", "
		<< "Error Page: " << it->second << std::endl;
	}
	os << "######################" << std::endl;
	for (size_t i = 0; i < rhs.getLocationVec().size(); i++) {
		os << rhs.getLocationVec()[i] << std::endl;
	}
	os << addColorText("########################", BLUE) << std::endl;
	return os;
}

std::ostream & operator<<(
	std::ostream & os ,
	const ServerConfig::Location & rhs)
{
	os << addColorText("Location", YELLOW) << std::endl;
	os << "path: " << rhs.getPath() << std::endl
	<< "alias: " << rhs.getAlias() << std::endl
	<< "index file: " << rhs.getIndexFile() << std::endl
	<< "upload place: " << rhs.getUploadPlace() << std::endl
	<< "return : <status: " << rhs.getReturn().first
	<< ", " << rhs.getReturn().second << ">" << std::endl
	<< "Allowe Method" << std::endl;
	for (std::map<std::string, bool>::const_iterator it = rhs.getAllowedMethod().begin();
		it != rhs.getAllowedMethod().end();
		it++)
	{
		os << it->first << ": " << it->second << std::endl;
	};
	for (size_t i = 0; i < rhs.getCgiExtensions().size(); i++)
	{
		os << "Extension: " << rhs.getCgiExtensions()[i] << std::endl;
	}
	return os;
}