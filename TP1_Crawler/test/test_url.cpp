#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../source/url.hpp"

int main(){
	char urls[10][100] = { 
		"http://www.decolar.com",
		"http://www.tripadvisor.com.br",
		"http://www.latam.com",
		"http://www.voeazul.com.br",
		"http://www.cvc.com.br",
		"http://directory.google.com/Top/Recreation/Outdoors/Hiking/Backpacking/",
		"http://username:password@www.chilkatsoft.com/purchase2.asp#buyZip",
		"http://www.chilkatsoft.com:80/purchase2.asp",
		"https://www.paypal.com:443/",
		"http://www.ChilkatSoft.com/"
	};

	CkSpider spider;
	Url url(&spider);

	for(int i=0; i<10; i++){
		printf("\n\nURL: %s", urls[i]);

		const char *canonized_url = url.getCleanUrl(urls[i]);
		printf("\ngetCleanUrl: %s", canonized_url);
		printf("\nisBrDomain: %s", url.isBrDomain(urls[i])? "true" : "false");

		printf("\ngetDomain: %s", url.getDomain(canonized_url));
		printf("\ngetNumberLevels: %d", url.getNumberLevels(canonized_url));
	}

	return 0;
}