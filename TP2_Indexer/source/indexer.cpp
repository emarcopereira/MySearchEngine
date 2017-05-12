#include "indexer.hpp"

using namespace std;

Indexer::Indexer(FILE *stop_words_file){

	/* Loading Stop Words */
	this->loadStopWords(stop_words_file);
}

void Indexer::loadStopWords(FILE *stop_words_file){
	char word[300];

	while(fscanf(stop_words_file, "%s", word) > 0){
		if(strlen(word) > 0){
			string aux = word;
			this->stop_words_table.add(aux);
		}
	}

	fprintf(stderr, "%lu stop words added.\n", this->stop_words_table.getSize());
}