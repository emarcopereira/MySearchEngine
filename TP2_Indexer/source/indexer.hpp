#ifndef _indexer_hpp
#define _indexer_hpp

#include <cstdio>

#include "stop_words_table.hpp"

class Indexer{

	private:
		StopWordsTable stop_words_table;

		void loadStopWords(FILE *stop_words_file);

	public:
		Indexer(FILE *stop_words_file);
};

#endif