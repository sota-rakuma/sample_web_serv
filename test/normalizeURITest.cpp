#include "Test.hpp"

void normalizeURITest()
{
	std::vector<std::string> test;
	test.push_back("example://a/b/c/%7Bfoo%7D");
	test.push_back("eXAMPLE://a/./b/../b/%63/%7bfoo%7d");
	HTTPRequest r1;
	HTTPRequest r2;
	r1.setRequestLine("", test[0], "");
	r2.setRequestLine("", test[1], "");
	if (r1.getRequestLine().getPath() == r2.getRequestLine().getPath())
	{
		std::cout << addColorText("OK", GREEN) << std::endl;
	} else {
		std::cout << addColorText("KO", RED) << std::endl;
	}
	std::cout << "r1: " << r1.getRequestLine().getPath() << std::endl;
	std::cout << "r2: " << r2.getRequestLine().getPath() << std::endl;
	test.push_back("/..");
	test.push_back("/sdaf/fds/..");
	test.push_back("/sdaf/fds/........../");
	test.push_back("/sdaf/fds/..........");
	test.push_back("/sdaf/fds/......../../");
	for (size_t i = 2; i < test.size(); i++)
	{
		r1.setRequestLine("", test[i], "");
		std::cout << r1.getRequestLine().getPath() << std::endl;
	}

}