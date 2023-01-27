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
		std::cerr << "例外: " << e.what() << std::endl;
	}
	return 0;
}