#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../source/url.hpp"

using namespace std;

int main(){
	char urls[11][100] = { 
		"http://www.decolar.com",
		"http://www.tripadvisor.com.br",
		"http://www.latam.com",
		"http://www.voeazul.com.br",
		"http://www.cvc.com.br",
		"http://directory.google.com/Top/Recreation/Outdoors/Hiking/Backpacking/",
		"http://username:password@www.chilkatsoft.com/purchase2.asp#buyZip",
		"http://www.chilkatsoft.com:80/purchase2.asp",
		"https://www.paypal.com:443/",
		"http://www.ChilkatSoft.com/",
		"http://www.ChilkatSoft.com/pt-br"
	};

	CkSpider spider;
	Url url(&spider);

	for(int i=0; i<11; i++){
		printf("\n\nURL: %s", urls[i]);

		string canonized_url = url.getCleanUrl(urls[i]);
		printf("\ngetCleanUrl: %s", canonized_url.c_str());

		printf("\nisBrDomain: %s", url.isBrDomain(urls[i])? "true" : "false");
		string domain = url.getDomain(canonized_url);
		printf("\ngetDomain: %s", domain.c_str());
		printf("\ngetHashDomain: %lu", url.getHashDomain(domain));

		printf("\ngetNumberLevels: %d", url.getNumberLevels(canonized_url));
	}

	return 0;
}