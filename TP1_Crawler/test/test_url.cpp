#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../source/url.hpp"

using namespace std;

int main(){
	char urls[15][100] = { 
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
		"http://www.ChilkatSoft.com/pt-br",
		"http://www.globo.com",
		"http://ufmg.br",
		"http://dcc.ufmg.br",
		"http://alteracaocadastral@pontosmultiplus.com.br"
	};

	CkSpider spider;
	Url url(&spider);
	CkString ckstr;

	for(int i=0; i<15; i++){
		fprintf(stderr, "\n\nURL: %s", urls[i]);

		bool sucess = url.getCleanUrl(urls[i], ckstr);
		string canonized_url = ckstr.getString();
		canonized_url.shrink_to_fit();
		fprintf(stderr, "\ngetCleanUrl(%d): %s", sucess, canonized_url.c_str());
		fprintf(stderr, "\nisBrDomain: %s", url.isBrDomain(urls[i])? "true" : "false");
		fprintf(stderr, "\ngetNumberLevels: %d", url.getNumberLevels(canonized_url));

		string domain = url.getDomain(ckstr.getString());
		domain.shrink_to_fit();
		fprintf(stderr, "\ngetDomain: %s", domain.c_str());
		fprintf(stderr, "\ngetHashDomain: %lu", url.getStringHash(domain));
		fprintf(stderr, "\ngetHashUrl: %lu", url.getStringHash(canonized_url));
	}

	return 0;
}