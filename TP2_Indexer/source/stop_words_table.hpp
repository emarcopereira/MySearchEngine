#ifndef _stop_words_table_hpp
#define _stop_words_table_hpp

#include <unordered_map>
#include <string>

class StopWordsTable{
	
	private:
		std::unordered_map<std::string, bool> stop_words;
		size_t occupied_space;

		inline void add(std::string &word){
			stop_words.emplace(word, true); occupied_space += word.size() + 1;
		}

	public:
		inline StopWordsTable(){
			occupied_space = 0;
		}

		void load(FILE *stop_words_file);

		inline bool constains(std::string &word){
			return (stop_words.count(word) > 0);
		}

		inline size_t getSize(){
			return stop_words.size();
		}

		inline size_t getOccupiedSpace(){
			return occupied_space;
		}
};

#endif