#ifndef SERVER_HPP
#define SERVER_HPP

#include "subject/EventMonitor.hpp"
#include "handler/ListenSocket.hpp"
#include "parser/ConfigParser.hpp"
#include <list>
#include <map>


class Server
{
private:
	EventMonitor _evm;
	ConfigParser _p;
	std::string _config_file;
	std::map<std::string, std::map<std::string, ServerConfig> > _confs;
	std::list<ListenSocket *> _ls;
	std::list<ICommand *> _commands;
private:
	void init();
	void setUpListenSocket();
public:
	Server();
	Server(const std::string &);
	void run();
	const std::string & getConfigPath() const;
	void setConfigPath(const std::string &);
	std::list<ListenSocket *>
	getListenSocks() const;
	void triggerEvent();
	~Server();
};

#endif /* SERVER_HPP */
