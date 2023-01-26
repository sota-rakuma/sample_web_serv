#include "ServerConfigFinder.hpp"
#include <fstream>

ServerConfigFinder::FileError::FileError()
:std::runtime_error("file error")
{
}

ServerConfigFinder::FileError::FileError(
	const std::string & err
)
:std::runtime_error(err)
{
}

ServerConfigFinder::FileError::~FileError() throw()
{
}

ServerConfigFinder::ServerConfigFinder()
{
}

ServerConfigFinder::ServerConfigFinder(
	const std::string & filename
)
{
	std::ifstream ifs(filename);
	std::string buf;

	if (ifs.fail() == true) {
		throw FileError("cannot open file");
	}
	if (std::getline(ifs, buf, '\0').fail() == true) {
		throw FileError("cannot read file by end");
	}
}

ServerConfigFinder::ServerConfigFinder(
	const ServerConfigFinder & another
)
:_infos(another._infos),
_keys(another._keys)
{
}

ServerConfigFinder::~ServerConfigFinder()
{
}

const std::vector<ServerConfig> &
ServerConfigFinder::getConfigVec() const
{
	return _infos;
}

const std::map<std::string, size_t>
ServerConfigFinder::getKeyMap() const
{
	return _keys;
}

const ServerConfig &
ServerConfigFinder::getConfig(
	const std::string & server_name
) const
{
	std::map<std::string, size_t>::const_iterator index = _keys.find(server_name);
	if (index == _keys.end()) {
		index = _keys.find("default");
	}
	if (index == _keys.end()) {
		throw std::runtime_error("default conf is not existed");
	}
	return _infos[index->second];
}

// void ServerConfigFinder::setFactor(const size_t server_index, const std::string value) {
// 	_infos[server_index]
// }