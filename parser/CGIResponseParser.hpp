#ifndef CGIRESPONSEPARSER_HPP
#define CGIRESPONSEPARSER_HPP

#include <iostream>
#include "TargetParser.hpp"
#include "Parser.hpp"
#include "../utils/utils.hpp"
#include "../HTTP/HTTPResponse.hpp"

enum ResponseType {
    DOCUMENT_RESPONSE = 0,
    LOCAL_REDIR_RESPONSE,
    CLIENT_REDIR_RESPONSE,
    CLIENT_REDIRDOC_RESPONSE,
};

class CGIResponseParser : public Parser {
    private:
        std::string _raw;
        size_t  _i;
        int _response_type;
        HTTPResponse *_http_res;
        ResponseType _res_type;
    public:
        CGIResponseParser();
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
        // int parseExtensionField();
        int parseResponseBody();
        int treatNL(std::string &, size_t &, size_t &);
};

#endif /* CGIRESPONSEPARSER_HPP */