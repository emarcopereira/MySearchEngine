#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <vector>
#include <string>
#include <cstring>

#include "crawler.hpp"

using namespace std;

Crawler *crawler;

void stopExecution(int signum){
	crawler->ForceStopCrawling();
}

int main(int argc, char *argv[]){
	if(argc != 23){
		fprintf(stderr, "\n\t./%s -s SEEDS_FILE_PATH -p PAGES_FILES_FOLDER_PATH -l LOGS_FILES_FOLDER_PATH -b BACKUP_FOLDER_PATH -s STATS_FILE_PATH -n N_THREADS \
					 -bp BUFFER_PAGES_SIZE(Bytes) -bui BUFFER_URLS_IN_SIZE(Qtd) -buo BUFFER_URLS_OUT_SIZE(Qtd) -bl BUFFER_LOG_SIZE(Bytes) -m EXECUTION_MODE", argv[0]);
		exit(-1);
	}


	char *seeds_file_path = argv[2];
	char *pages_folder_path = argv[4];
	char *logs_folder_path = argv[6];
	char *backup_folder_path = argv[8];
	char *stats_file_path = argv[10];
	char *execution_mode = argv[22];

	const int n_threads = atoi(argv[12]);
	const int buffer_pages_size = atoi(argv[14]);
	const int buffer_iUrls_size = atoi(argv[16]);
	const int buffer_oUrls_size = atoi(argv[18]);
	const int buffer_log_size = atoi(argv[20]);

	/* Reading Seeds File */
	FILE *seeds_file = fopen(seeds_file_path, "r");
	if(seeds_file == NULL){
		fprintf(stderr, "\n\tError opening input file '%s'", seeds_file_path);
		exit(EXIT_FAILURE);
	}

	vector<string> seeds;
	char url[300];
	while(fscanf(seeds_file, "%s", url) > 0){
		if(strlen(url) > 2)
			seeds.push_back(string(url));
	}

	fclose(seeds_file);

	signal(SIGINT, stopExecution);

	/* Executing Crawler */
	if(!strcmp(execution_mode, "complex"))
		crawler = new Crawler(pages_folder_path, logs_folder_path, backup_folder_path, stats_file_path, buffer_pages_size, buffer_iUrls_size, buffer_oUrls_size, buffer_log_size);

	crawler->Initialize(seeds);

	crawler->StartCrawling(n_threads);

	crawler->StopCrawling();
	
	delete crawler;

	return EXIT_SUCCESS;
}