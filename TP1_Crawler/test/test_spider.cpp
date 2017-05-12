#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <random>
#include "../source/spider.hpp"
#include <chrono>

using namespace std;

int main(){
	char urls[8][100] = { 
		"http://www.decolar.com",
		"http://www.tripadvisor.com.br",
		"http://www.latam.com",
		"http://www.voeazul.com.br",
		"http://www.cvc.com.br",
		"http://www.globo.com",
		"http://ufmg.br",
		"http://dcc.ufmg.br"
	};

	CkSpider ck_spider;
	Url url(&ck_spider);
	CkString ckstr;

	chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
	Logger logger(0, "test", "test.logger", 500000, start_time);
	Dumper dumper("test", "test.dump", 5000000);
	Spider spider(1000, 1000, &logger, &dumper);

	random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 generator(rd());
	uniform_int_distribution<int> distribution(1,8);
	int rand = distribution(generator);

	char *c_url = "http://tenso.blog.br/";

	bool sucess = url.getCleanUrl(c_url, ckstr);
	string canonized_url = ckstr.getString();
	canonized_url.shrink_to_fit();
	int n_levels = url.getNumberLevels(canonized_url);
	fprintf(stderr, "\ngetCleanUrl(%d): %s", sucess, canonized_url.c_str());
	fprintf(stderr, "\nisBrDomain: %s", url.isBrDomain(c_url)? "true" : "false");
	fprintf(stderr, "\ngetNumberLevels: %d", n_levels);

	string domain = url.getDomain(ckstr.getString());
	domain.shrink_to_fit();
	size_t domain_id = url.getStringHash(domain);
	size_t url_id = url.getStringHash(canonized_url);
	fprintf(stderr, "\ngetDomain: %s", domain.c_str());
	fprintf(stderr, "\ngetHashDomain: %lu", domain_id);
	fprintf(stderr, "\ngetHashUrl: %lu", url_id);

	spider.collect(UrlCarrier(canonized_url, url_id, domain, domain_id, n_levels));

	return 0;
}