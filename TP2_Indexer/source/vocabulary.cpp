#include "vocabulary.hpp"

void Vocabulary::write(FILE *out_file){
	fprintf(out_file, "%lu\n", this->term_index.size());

	for(auto item : this->term_index){
		fprintf(out_file, "%s %d\n", item.first.c_str(), item.second);
	}
}