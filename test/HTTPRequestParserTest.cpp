#include "Test.hpp"

static void requestLineNormalTest()
{
	std::vector<std::string> normal;
	normal.push_back("GET / HTTP/1.1");
	normal.push_back("fasffds / HTTP/1.1");
	normal.push_back("GET /../..../ HTTP/1.1");
	normal.push_back("GET http://a/ HTTP/1.1");
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
}

static void requestLineAbNormalTest()
{
	HTTPRequest req;
	HTTPRequestParser p(&req);
	std::vector<std::string> abnormal;
	abnormal.push_back("");
	abnormal.push_back("GET/HTTP/1.1");
	abnormal.push_back("GET /HTTP/1.1");
	abnormal.push_back("GET  HTTP/1.1");
	abnormal.push_back("@@@ / HTTP/1.1");
	abnormal.push_back("GET fsdfsda HTTP/1.1");
	abnormal.push_back("GET / HTTP/11");
	abnormal.push_back("\r\n\r\n\r\nGET / HTTP/11");
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

void requestLineTest()
{
	requestLineNormalTest();
	requestLineAbNormalTest();
}

std::string changeNewLineCode(const std::string & raw)
{
	std::string output;
	for (size_t i = 0; i < raw.size(); i++)
	{
		if (raw[i] == '\r') {
			output += "\\r";
		} else if (raw[i] == '\n') {
			output += "\\n";
		} else {
			output += raw[i];
		}
	}
	return output;
}

static void headerNormalTest()
{
	std::vector<std::string> normal;
	normal.push_back("Host: localhost\r\n\r\n");
	normal.push_back("Host: a, b\r\n\r\n");
	normal.push_back("host: a\r\nHOst: b\r\n\r\n");
	normal.push_back("Host: localhost\r\nTransfer-Encodind: gzip, chunked\r\n");
	normal.push_back("Host: localhost\r\nTransfer-Encodind: gzip, chunked\r\nhost: a,       b\r\n\r\n");
	normal.push_back("Host:\r\n\r\n");
	normal.push_back("Host: あfds\r\n\r\n");
	normal.push_back("host: localhost\r\n		fasfsad\r\n");
	normal.push_back("");
	HTTPRequestParser p;
	p.setState(HEADER_FIELD);
	std::cout << addColorText("normal test: ", GREEN) << std::endl;
	for (size_t i = 0; i < normal.size(); i++)
	{
		HTTPRequest req;
		p.setRequest(&req);
		std::cout << "##### " << changeNewLineCode(normal[i]) << " #####" << std::endl;
		if (p.parse(normal[i]) == -1) {
			std::cout << addColorText("ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("OK", GREEN) << std::endl;
		}
		const std::__1::map<std::__1::string, std::__1::string> & h = req.getHeaderField();
		for (std::__1::map<std::__1::string, std::__1::string>::const_iterator it = h.begin();
			it != h.end();
			it++)
		{
			std::cout << "NAME: " << it->first << std::endl
			<< "VALUE: " << it->second << std::endl;
		}
	}
}

static void headerAbNormalTest()
{
	HTTPRequestParser p;
	std::vector<std::string> abnormal;
	p.setState(HEADER_FIELD);
	abnormal.push_back("host : localhost\r\n");
	abnormal.push_back("host  localhost\r\n");
	abnormal.push_back("\r\nhost: localhost, fasfsad\r\n");
	abnormal.push_back("@@@@ : localhost\r\n");
	std::cout << addColorText("abnormal test: ", RED) << std::endl;
	for (size_t i = 0; i < abnormal.size(); i++)
	{
		HTTPRequest req;
		p.setRequest(&req);
		std::cout << "##### " << changeNewLineCode(abnormal[i]) << " #####" << std::endl;
		if (p.parse(abnormal[i]) == 0) {
			std::cout << addColorText("ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("OK", GREEN) << std::endl;
		}
	}
}

void headerFieldTest()
{
	headerNormalTest();
	headerAbNormalTest();
}