#include "Test.hpp"

int main(void)
{
	TargetParserTest();
	//requestLineTest();
	//headerFieldTest();
	//normalizeURITest();
	// ConfigParserTest();
	//CGIResponseParserTest();
	return 0;
}

// ↓ConfigParserTest時のコンパイル用↓
// c++ ConfigParserTest.cpp ../parser/ConfigParser.cpp ../config/ServerConfig.cpp ../config/ServerConfigFinder.cpp ../parser/TargetParser.cpp ../parser/Parser.cpp ../utils/isAlpha.cpp ../utils/isLowerCase.cpp ../utils/isUpperCase.cpp ../utils/isHexDig.cpp ../utils/isDigit.cpp Test.cpp -o cp

// ↓CGIResponseParserTest時のコンパイル用↓
// c++ CGIResponseParserTest.cpp Test.cpp ../parser/CGIResponseParser.cpp ../parser/Parser.cpp ../parser/TargetParser.cpp ../HTTP/HTTPResponse.cpp ../utils/isAlpha.cpp ../utils/isDigit.cpp ../utils/isPrintable.cpp ../utils/isLowerCase.cpp ../utils/isUpperCase.cpp ../utils/isHexDig.cpp ../utils/addColorText.cpp -o cgip