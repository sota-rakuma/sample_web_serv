#include "Test.hpp"

int main(void)
{
	//TargetParserTest();
	// requestLineTest();
	// ConfigParserTest();
	CGIResponseParserTest();
}

// ↓ConfigParserTest時のコンパイル用↓
// c++ ConfigParserTest.cpp ../parser/ConfigParser.cpp ../config/ServerConfig.cpp ../config/ServerConfigFinder.cpp ../parser/TargetParser.cpp ../parser/Parser.cpp ../utils/isAlpha.cpp ../utils/isLowerCase.cpp ../utils/isUpperCase.cpp ../utils/isHexDig.cpp ../utils/isDigit.cpp Test.cpp -o cp


// ↓CGIResponseParserTest時のコンパイル用↓
// c++ CGIResponseParserTest.cpp ../parser/CGIResponseParser.cpp ../parser/Parser.cpp Test.cpp -o cgirp