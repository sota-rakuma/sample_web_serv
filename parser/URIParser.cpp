#include "URIParser.hpp"

int main(void)
{
	URIParser _p;
	std::string test1 = "http://www.ics.uci.edu/pub/ietf/uri/?aaaa#Related";
	std::string test2 = "://?#";

	_p.parse(test2);
	return 0;
}