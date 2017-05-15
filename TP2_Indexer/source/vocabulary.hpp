#ifndef _vocabulary_hpp
#define _vocabulary_hpp

#include <cstdio>

#include <unordered_map>
#include <string>

class Vocabulary{

	private:

		std::unordered_map<std::string, int> term_index;
		size_t occupied_space;

	public:

		inline Vocabulary(){
			occupied_space = 0;
		}

		inline int getSize(){
			return term_index.size();
		 }

		inline size_t getOccupiedSpace(){
			return occupied_space;
		}

		int addTerm(std::string &term);

		inline int getTermId(std::string &term){
			return term_index[term];
		}

		void save(FILE *out_file);

		void load(FILE *vocabulary_file);

		inline void clear(){
			term_index.clear(); occupied_space = 0;
		}

		/*if(vocabulary.addTerm(term)){
			working_memory -= term.size();
			vocabulary.getSize() += term.size();
		}*/
};

#endif