#ifndef CGIRESPONSEPARSER_HPP
#define CGIRESPONSEPARSER_HPP

#include "Parser.hpp"
#include <iostream>

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
        std::string _content_type;
    public:
        CGIResponseParser();
        virtual ~CGIResponseParser();
        virtual int parse(const std::string &);
        int getResponseType() const;
        int parseDocumentResponse();
        int parseLocalRedirResponse();
        int parseClientRedirResponse();
        int parseClientRedirDocResponse();
        int parseContentType();
        int parseMediaType();
        int parseStatus();
        int parseOtherField();
        int parseResponseBody();
        int parseClientLocation();
        int parseExtensionField();
};

#endif /* CGIRESPONSEPARSER_HPP */