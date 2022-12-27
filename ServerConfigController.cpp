#include "ServerConfigController.hpp"
#include <fstream>

ServerConfigController::FileError::FileError()
:std::runtime_error("file error")
{
}

ServerConfigController::FileError::FileError(
	const std::string & err
)
:std::runtime_error(err)
{
}

ServerConfigController::FileError::~FileError()
{
}

ServerConfigController::ServerConfigController()
{
}

ServerConfigController::ServerConfigController(
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

ServerConfigController::ServerConfigController(
	const ServerConfigController & another
)
:_infos(another._infos),
_keys(another._keys)
{
}

ServerConfigController::~ServerConfigController()
{
}

const std::vector<ServerConfig> &
ServerConfigController::getConfigVec() const
{
	return _infos;
}

const std::map<std::string, size_t>
ServerConfigController::getKeyMap() const
{
	return _keys;
}

const ServerConfig &
ServerConfigController::getConfig(
	const std::string & server_name
) const
{
	size_t index = _keys.at(server_name);
	return _infos[index];
}