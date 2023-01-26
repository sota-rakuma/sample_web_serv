#include "Server.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	Server sv;
	if (argc != 2) {
		sv.setConfigPath("test.conf");
	} else {
		sv.setConfigPath(argv[1]);
	}
	try
	{
		sv.run();
	}
	catch(const std::exception& e)
	{
		for (std::list<ListenSocket>::const_iterator it = sv.getListenSocks().begin();
		it != sv.getListenSocks().end();
		it++)
		{
			it->close();
		}
	}
	return 0;
}