#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>

#include "stop_words_table.hpp"
#include "vocabulary.hpp"
#include "indexer.hpp"
#include "logger.hpp"
#include "searcher.hpp"
#include "compressor.hpp"

using namespace std;

FILE *openFile(char *file_path, const char *open_mode);

void loadHtmlsFilesList(FILE *htmls_files_list_file, vector<string> &htmls_files_list);

int main(int argc, char *argv[]){
	if(!strcmp(argv[2], "INDEX") && argc == 17){
		char *htmls_files_list_path = argv[4], *stopwords_file_path = argv[6], *output_file_path = argv[8],
			 *vocabulary_file_path = argv[10], *runs_folder_path = argv[12], *log_file_path = argv[14];
		const int memory_limit = atoi(argv[16]);

		/* Loading Htmls Files List */
		vector<string> htmls_files_list;
		FILE *htmls_files_list_file = openFile(htmls_files_list_path, "r");
		loadHtmlsFilesList(htmls_files_list_file, htmls_files_list);
		fclose(htmls_files_list_file);

		/* Loading Stop Words */
		StopWordsTable stop_words_table;
		FILE *stopwords_file = openFile(htmls_files_list_path, "r");
		stop_words_table.load(stopwords_file);
		fclose(stopwords_file);

		/* Opening Output Files */
		FILE *index_file = openFile(output_file_path, "w");
		FILE *vocabulary_file = openFile(vocabulary_file_path, "w");

		Vocabulary vocabulary;

		Indexer indexer;

		fclose(index_file);
		fclose(vocabulary_file);
	}
	else if(!strcmp(argv[2], "SEARCH") && argc == 11){
		char *index_file_path = argv[4], *stopwords_file_path = argv[6], *vocabulary_file_path = argv[8],
			 *log_file_path = argv[10];

	}
	if(!strcmp(argv[2], "COMPRESSION") && argc == 13){
		char *index_file_path = argv[4], *vocabulary_file_path = argv[6], *output_file_path = argv[8],
			 *compression_method = argv[10], *log_file_path = argv[12];

	}
	if(!strcmp(argv[2], "DECOMPRESSION") && argc == 13){
		char *compressed_index_file_path = argv[4], *vocabulary_file_path = argv[6], *output_file_path = argv[8],
			 *compression_method = argv[10], *log_file_path = argv[12];
	}
	else{
		fprintf(stderr, "\n\t./%s -t <task> [parameters] \n\
						task: INDEX parameters: -h <htmls_files_list_path> -s <stopwords_file_path> -o <output_file_path> -v <vocabulary_file_path> -r <runs_folder_path> -l <log_file_path> -m <memory_limit> \n\
					 	task: SEARCH parameters: -i <index_file_path> -v <vocabulary_file_path> -o <output_file_path> -l <log_file_path> \n\
					 	task: COMPRESSION parameters: -i <index_file_path> -v <vocabulary_file_path> -o <output_file_path> -c <compression_method [ELIAS_GAMA | GULOMB]> -l <log_file_path> \n\
					 	task: DECOMPRESSION parameters: -i <compressed_index_file_path> -v <vocabulary_file_path> -o <output_file_path> -c <compression_method [ELIAS_GAMA | GULOMB]> -l <log_file_path>", argv[0]);
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

FILE *openFile(char *file_path, const char *open_mode){
	FILE *file = fopen(file_path, open_mode);
	if(file == NULL){
		fprintf(stderr, "\n\tError opening input file '%s'\n", file_path);
		exit(EXIT_FAILURE);
	}
	return file;
}

void loadHtmlsFilesList(FILE *htmls_files_list_file, vector<string> &htmls_files_list){
	char word[300];
	while(fscanf(htmls_files_list_file, "%s", word) > 0){
		if(strlen(word) > 0){
			string aux = word;
			htmls_files_list.push_back(aux);
		}
	}
	fprintf(stderr, "%lu filenames added.\n", htmls_files_list.size());
}