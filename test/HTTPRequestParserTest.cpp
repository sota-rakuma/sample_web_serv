#include "Test.hpp"

void requestLineTest()
{
	std::vector<std::string> normal;
	normal.push_back("GET / HTTP/1.1");
	HTTPRequest req;
	HTTPRequestParser p(&req);
		std::cout << addColorText("normal test: ", GREEN) << std::endl;
	for (size_t i = 0; i < normal.size(); i++)
	{
		std::cout << "##### " << normal[i] << " #####" << std::endl;
		if (p.parse(normal[i]) == -1) {
			std::cout << addColorText("ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("OK", GREEN) << std::endl;
		}
	}

	return ;

	std::vector<std::string> abnormal;
	abnormal.push_back("");
	std::cout << addColorText("abnormal test: ", RED) << std::endl;
	for (size_t i = 0; i < abnormal.size(); i++)
	{
		std::cout << "##### " << abnormal[i] << " #####" << std::endl;
		if (p.parse(abnormal[i]) == 0) {
			std::cout << addColorText("ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("OK", GREEN) << std::endl;
		}
	}
}

void headerFieldTest()
{
	std::vector<std::string> normal;
	normal.push_back("");
	HTTPRequest req;
	HTTPRequestParser p(&req);
	p.setState(HEADER_FIELD);
		std::cout << addColorText("normal test: ", GREEN) << std::endl;
	for (size_t i = 0; i < normal.size(); i++)
	{
		std::cout << "##### " << normal[i] << " #####" << std::endl;
		if (p.parse(normal[i]) == -1) {
			std::cout << addColorText("ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("OK", GREEN) << std::endl;
		}
	}

	std::vector<std::string> abnormal;
	abnormal.push_back("");
	std::cout << addColorText("abnormal test: ", RED) << std::endl;
	for (size_t i = 0; i < abnormal.size(); i++)
	{
		std::cout << "##### " << abnormal[i] << " #####" << std::endl;
		if (p.parse(abnormal[i]) == 0) {
			std::cout << addColorText("ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("OK", GREEN) << std::endl;
		}
	}
}