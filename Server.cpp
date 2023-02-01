#include "Server.hpp"
#include <unistd.h>
Server::Server()
{
}

Server::Server(const std::string & config_file)
:_config_file(config_file)
{
}

Server::~Server()
{
}

void Server::run()
{
	if (init() == -1) {
		throw std::runtime_error("Server Initiation failed");
	}
	while (true)
	{
		if (_evm.monitor() == -1) {
			throw std::runtime_error("Event Monitor Error");
		}
		if (_commands.size() == 0) {
			usleep(100);
			continue;
		}
		triggerEvent();
	}
}

int Server::init()
{
	if (_p.parse(_config_file) == 1) {
		return -1;
	}
	const std::vector<ServerConfig> & configs = _p.getScVec();
	for (size_t i = 0; i < configs.size(); i++) {
		const std::string & lis = configs[i].getListen();
		_confs[lis].insert(std::make_pair(configs[i].getServerName(), configs[i]));
	}
	try
	{
		setUpListenSocket();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return -1;
	}
	return 0;
}

void Server::setUpListenSocket()
{
	for (std::map<std::string, std::map<std::string, ServerConfig> >::iterator it = _confs.begin();
		it != _confs.end();
		it++)
	{
		_ls.push_back(new ListenSocket(&_evm, &_commands, "127.0.0.1", it->first, &it->second));
	}
}

void Server::triggerEvent()
{
	ICommand *temp = static_cast<ICommand *>(NULL);
	while (_commands.size() > 0) {
		temp = *(_commands.begin());
		(*(_commands.begin()))->execute();
		if (temp == *(_commands.begin())) {
			_commands.pop_front();
		}
	}
}

const std::string & Server::getConfigPath() const
{
	return _config_file;
}

void Server::setConfigPath(const std::string & conf)
{
	_config_file = conf;
}

std::list<ListenSocket *>
Server::getListenSocks() const {
	return _ls;
}