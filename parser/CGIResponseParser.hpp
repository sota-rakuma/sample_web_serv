#ifndef CGIRESPONSEPARSER_HPP
#define CGIRESPONSEPARSER_HPP

#include <iostream>
#include "TargetParser.hpp"
#include "Parser.hpp"
#include "../utils/utils.hpp"
#include "../HTTP/HTTPResponse.hpp"

#ifndef DOCUMENT_RESPONSE
#define DOCUMENT_RESPONSE 1
#endif

#ifndef LOCAL_REDIR_RESPONSE
#define LOCAL_REDIR_RESPONSE 2
#endif

#ifndef CLIENT_REDIR_RESPONSE
#define CLIENT_REDIR_RESPONSE 4
#endif

#ifndef CLIENT_REDIRDOC_RESPONSE
#define CLIENT_REDIRDOC_RESPONSE 8
#endif

#ifndef ERROR
#define ERROR -1
#endif

class CGIResponseParser : public Parser {
	private:
		std::string _raw;
		std::string _raw_low;
		size_t  _i;
		int _res_type;
		HTTPResponse *_http_res;
	public:
		CGIResponseParser();
		CGIResponseParser(
			HTTPResponse *
		);
		virtual ~CGIResponseParser();
		virtual int parse(const std::string &);
		int getResponseType();
		int parseDocumentResponse();
		int parseLocalRedirResponse();
		int parseClientRedirResponse();
		int parseClientRedirDocResponse();
		int parseContentType();
		int parseStatus();
		int parseOtherField();
		int parseClientLocation();
		int parseResponseBody();
		int treatNL(std::string &, size_t &, size_t &);
		HTTPResponse *getHTTPResponse() const;
};

#endif /* CGIRESPONSEPARSER_HPP */