#include "Test.hpp"

void CGIResponseParserTest() {
    CGIResponseParser cgirp;
    std::string raw = "abc\ndef"; //
    if (cgirp.parse(raw) != 0) {
        std::cout << "parse() in CGIResponseParserTest failed" << std::endl;
        return;
    }
    std::cout << "parse() successed" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "raw = " << raw << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}