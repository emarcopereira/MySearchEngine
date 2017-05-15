#include "vocabulary.hpp"

using namespace std;

int Vocabulary::addTerm(string &term){
	auto answer = term_index.emplace(term, term_index.size());
	if(answer.second) occupied_space += term.size() + 4;
	return answer.first->second;
}

void Vocabulary::save(FILE *out_file){
	size_t size = this->term_index.size();
	fprintf(out_file, "%lu\n", size);

	for(auto item : this->term_index){
		fprintf(out_file, "%s %d\n", item.first.c_str(), item.second);
	}

	fprintf(stderr, "%lu terms saved from vocabulary.\n", size);
}


void Vocabulary::load(FILE *vocabulary_file){
	size_t size;
	int index;
	char str[300];
	fscanf(vocabulary_file, "%lu", &size);

	for(size_t i=0; i<size; i++){
		fscanf(vocabulary_file, "%s %d\n", str, &index);
		string term = str;
		this->term_index.emplace(term, index);
	}

	fprintf(stderr, "%lu terms added in vocabulary.\n", this->term_index.size());
}