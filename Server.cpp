#include "Server.hpp"
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
	init();
	for (std::list<ListenSocket>::iterator it = _ls.begin();
		it != _ls.end();
		it++)
	{
		std::cout << *it << std::endl;
	}
	std::exit(0);
	while (true)
	{
		if (_evm.monitor() == -1) {
			throw std::runtime_error("Event Monitor Error");
		}
		triggerEvent();
	}
}

void Server::init()
{
	if (_p.parse(_config_file) == -1) {
		return ;
	}
	const std::vector<ServerConfig> & configs = _p.getScVec();
	for (size_t i = 0; i < configs.size(); i++) {
		const std::string & lis = configs[i].getListen();
		_confs[lis].insert(std::make_pair(configs[i].getServerName(), configs[i]));
	}
	setUpListenSocket();
}

void Server::setUpListenSocket()
{
	for (std::map<std::string, std::map<std::string, ServerConfig> >::iterator it = _confs.begin();
		it != _confs.end();
		it++)
	{
		_ls.push_back(ListenSocket(&_evm, &_commands, "127.0.0.1", it->first, &it->second));
	}
}

void Server::triggerEvent()
{
	while (_commands.size() > 0) {
		(*(_commands.begin()))->execute();
		_commands.pop_front();
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

const std::list<ListenSocket> &
Server::getListenSocks() const{
	return _ls;
}