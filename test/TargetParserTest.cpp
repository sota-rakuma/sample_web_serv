#include <vector>
#include "Test.hpp"
#include "../parser/TargetParser.hpp"

void addNormalCase(
	std::vector<std::string> & test
)
{
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http://127.0.0.1:8080/pub/ietf/uri/#Related");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]:8080");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]:");
	test.push_back("http://[::3:1000:100:20:3]:8080");
	test.push_back("http://[::3:1000:100:127.0.0.1]:8080");
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/?query#Related");
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/?query");
	test.push_back("http://[::3:1000:100:127.0.0.1]:8080?query");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/?query");
}

void addAbnormalCase(
	std::vector<std::string> & test
)
{
	test.push_back("");
	test.push_back("://www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http//www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http:/www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http//www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3");
	test.push_back("http://2001:db8:20:3:1000:100:20:3");
	test.push_back("http:///");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/#Related");
}

void TargetParserTest(void)
{
	std::vector<std::string> test;

// 正常系
	//addNormalCase(test);
// 異常系
	addAbnormalCase(test);


	for (size_t i = 0; i < test.size(); i++)
	{
		TargetParser p;
		std::cout << "test[" << i << "]: " << test[i] << std::endl;
		if (p.parse(test[i]) == -1) {
			std::cout << addColorText("PARSER ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("PARSE SUCCESS", GREEN) << std::endl;
		}
		std::cout << p << std::endl << std::endl;
	}
}
