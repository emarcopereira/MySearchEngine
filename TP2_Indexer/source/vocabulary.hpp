#ifndef _vocabulary_hpp
#define _vocabulary_hpp

#include <cstdio>

#include <unordered_map>
#include <string>

class Vocabulary{

	private:

		std::unordered_map<std::string, int> term_index;

		size_t size;

	public:

		Vocabulary(){ }

		inline bool addTerm(std::string &term){
			return term_index.emplace(term, term_index.size()).second;
		}

		inline int getSize(){ return this->size; }

		void write(FILE *out_file);

		/*if(vocabulary.addTerm(term)){
			working_memory -= term.size();
			vocabulary.getSize() += term.size();
		}*/
};

#endif