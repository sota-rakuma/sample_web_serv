#include "Test.hpp"

void addNormalCase(
	std::vector<std::string> & test
)
{
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http://127.0.0.1:8080/pub/ietf/uri/#Related");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]:8080");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3]:");
	test.push_back("http://[::3:1000:100:20:3]:8080");
	test.push_back("http://[::3:1000:100:127.0.0.1]:8080");
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/?query#Related");
	test.push_back("http://www.ics.uci.edu/pub/ietf/uri/?query");
	test.push_back("http://[::3:1000:100:127.0.0.1]:8080?query");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/?query");
	test.push_back("/");
}

void addAbnormalCase(
	std::vector<std::string> & test
)
{
	test.push_back("");
	test.push_back("://www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http//www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http:/www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http//www.ics.uci.edu/pub/ietf/uri/#Related");
	test.push_back("http://[2001:db8:20:3:1000:100:20:3");
	test.push_back("http://2001:db8:20:3:1000:100:20:3");
	test.push_back("http:///");
	test.push_back("/www.ics.uci.edu/pub/ietf/uri/#Related");
}
/*
全て等しい値になってるか要チェック
1.
http://example.com:80/~smith/home.html
http://EXAMPLE.com/%7Esmith/home.html
http://EXAMPLE.com:/%7esmith/home.html

2.
example://a/b/c/%7Bfoo%7D
eXAMPLE://a/./b/../b/%63/%7bfoo%7d
*/

/*
URIの正規化
・scheme, hostに関しては全て小文字で処理をする(他は区別する)
・パーセントエンコーディングをデコードする
・パスセグメント中に".", ".." が現れたら、それらに適切な処理を行う
・pathが空っぽなら、それに / をつけてルートとして処理を行う
*/
void TargetParserTest()
{
	std::vector<std::string> test;

// 正常系
	addNormalCase(test);
// 異常系
	//addAbnormalCase(test);

	for (size_t i = 0; i < test.size(); i++)
	{
		TargetParser p;
		std::cout << "test[" << i << "]: " << test[i] << std::endl;
		if (p.parse(test[i]) == -1) {
			std::cout << addColorText("PARSER ERROR", RED) << std::endl;
		} else {
			std::cout << addColorText("PARSE SUCCESS", GREEN) << std::endl;
		}
		std::cout << p << std::endl << std::endl;
	}
}

