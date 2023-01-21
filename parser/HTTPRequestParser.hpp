#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include "TargetParser.hpp"
#include "HTTPRequest.hpp"

enum RequestParsingState {
	REQUEST_LINE,
	HEADER_FIELD,
};

class HTTPRequestParser : public Parser
{
private:
	HTTPRequest *_req;
	TargetParser _tp;
	RequestParsingState _state;
private:
	int parseMethod(const std::string &);
	int parseRequestTarget(
		const std::string &
	);
	int parseHTTPVersion(
		const std::string &
	);
public:
	HTTPRequestParser();
	HTTPRequestParser(HTTPRequest *);
	HTTPRequestParser(const HTTPRequestParser &);
	~HTTPRequestParser();
	virtual int parse(const std::string &);
	int parseRequestLine(const std::string &);
	int parseHeaderFiled(const std::string &);
	HTTPRequest *&getRequest();
	RequestParsingState getState() const;
	static bool isToken(
		const std::string &
	);
	static bool isTcahr(char c);
};

#endif /* HTTPREQUESTPARSER_HPP */