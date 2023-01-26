#include "Test.hpp"
#include <time.h>
#include <sys/time.h>

void CGIResponseParserTest() {
    CGIResponseParser cgip;
    std::string raw = "Content-Type: abc\nStatus:  981234 6digit\nabcd: llll\nServer:    \nServer:   abcd\nabcd: zzz\n\nHello\n"; // document_response
    // std::string raw = "Location: /a>><''bc/de32@f/ghi\n\n"; // local_redir_response
    // std::string raw = "Location:  http://abc/def\nabcd: efg\n\n"; // client_redir_response
    // std::string raw = "Location: http://zzz/xxx\nStatus: 9999 4digit\nContent-Type: abc/def\nServer: yyy\n\nHello\n"; // client_redirdoc_response

    // struct timeval startTime;
    // struct timeval endTime;
    // gettimeofday(&startTime, NULL);
    // std::cout << startTime.tv_usec << std::endl;
    if (cgip.parse(raw) != 0) {
        std::cout << "parse() in CGIResponseParserTest failed" << std::endl;
        return;
    }
    // gettimeofday(&endTime, NULL);
    // std::cout << endTime.tv_usec << std::endl;
    std::cout << "parse() successed" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "raw = " << raw << std::endl;
    std::cout << "----------------StatusLine---------------------" << std::endl;
    std::cout << cgip.getHTTPResponse()->getStatusLine().getHTTPVersion() << " " \
        << cgip.getHTTPResponse()->getStatusLine().getCode() << " " \
        << cgip.getHTTPResponse()->getStatusLine().getReason() << std::endl;
    std::cout << "----------------HeaderFields-------------------" << std::endl;
    // std::cout << "Content-Type: " << cgip.getHTTPResponse()->getHeaderValue("Content-Type") << std::endl;
    // std::cout << "abcd: " << cgip.getHTTPResponse()->getHeaderValue("abcd") << std::endl;
    // std::cout << "Server: " << cgip.getHTTPResponse()->getHeaderValue("Server") << std::endl;
    std::cout << "Location: " << cgip.getHTTPResponse()->getHeaderValue("Location") << std::endl;
    std::cout << "----------------MessageBody--------------------" << std::endl;
    // std::cout << cgip.getHTTPResponse()->getMessageBody() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}