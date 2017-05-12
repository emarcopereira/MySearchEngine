#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <vector>
#include <string>
#include <cstring>
#include <random>
#include <chrono>

#include "../source/crawler.hpp"

using namespace std;

Crawler *crawler;

void stopExecution(int signum){
	crawler->ForceStopCrawling();
}

int main(int argc, char *argv[]){
	/*FILE *seeds_file = fopen("input/seeds.txt", "r");
	vector<string> seeds;
	char url[300];
	while(fscanf(seeds_file, "%s", url) > 0){
		if(strlen(url) > 2)
			seeds.push_back(string(url));
	}
	fclose(seeds_file);*/

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

	random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 generator(rd());
	uniform_int_distribution<int> distribution(1,8);
	int rand = distribution(generator);

	signal(SIGINT, stopExecution);

	/* Executing Crawler */
	crawler = new Crawler("test/test_output/documents", "test/test_output/logs", "test/test_output/backup", "test/test_output/test.stats",
				 5000000, 100, 500, 500000);

	vector<string> seeds;
	seeds.push_back(string(urls[rand-1]));

	chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

	crawler->Initialize(seeds);

	fprintf(stderr, "\nBEFORE START\n");

	for(int i=0; i<MAX_URL_LEVEL; i++)
		fprintf(stderr, "%d:%lu ", i+1, crawler->urls_levels[i].url_queue->size());
	fprintf(stderr, "\n");
	for(int i=0; i<MAX_URL_LEVEL; i++)
		fprintf(stderr, "%d:%lu ", i+1, crawler->urls_levels[i].url_hash->size());
	fprintf(stderr, "\n\n");

	for(auto &p : crawler->domain_times_hash)
		fprintf(stderr, "%lu:%ld ", p.first, chrono::duration_cast<chrono::seconds>(p.second - start_time));
	fprintf(stderr, "\n");

	crawler->StartCrawling(80);

	fprintf(stderr, "\nAFTER START\n");

	for(int i=0; i<MAX_URL_LEVEL; i++)
		fprintf(stderr, "%d:%lu ", i+1, crawler->urls_levels[i].url_queue->size());
	fprintf(stderr, "\n");
	for(int i=0; i<MAX_URL_LEVEL; i++)
		fprintf(stderr, "%d:%lu ", i+1, crawler->urls_levels[i].url_hash->size());
	fprintf(stderr, "\n\n");

	for(auto &p : crawler->domain_times_hash)
		fprintf(stderr, "%lu:%ld ", p.first, chrono::duration_cast<chrono::seconds>(p.second - start_time));
	fprintf(stderr, "\n");


	crawler->StopCrawling();

	delete crawler;

	return EXIT_SUCCESS;
}