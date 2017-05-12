#ifndef _stop_words_table_hpp
#define _stop_words_table_hpp

#include <unordered_map>
#include <string>

class StopWordsTable{
	
	private:
		std::unordered_map<std::string, bool> stop_words;

	public:
		StopWordsTable(){}

		inline void add(std::string &word){
			stop_words.emplace(word, true);
		}

		inline bool constains(std::string &word){
			return (stop_words.count(word) > 0);
		}

		inline size_t getSize(){
			return stop_words.size();
		}
};

#endif