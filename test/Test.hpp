#ifndef TEST_HPP
#define TEST_HPP

#include "../utils/utils.hpp"
#include "../parser/HTTPRequestParser.hpp"
#include "../parser/TargetParser.hpp"
#include "../parser/ConfigParser.hpp"
#include "../parser/CGIResponseParser.hpp"
#include <vector>

void TargetParserTest();
void requestLineTest();
void headerFieldTest();
void normalizeURITest();
void ConfigParserTest();
void CGIResponseParserTest();

#endif /* TEST_HPP */
