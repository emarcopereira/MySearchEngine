#ifndef _indexer_hpp
#define _indexer_hpp

#include <cstdio>
#include <fstream>

#include <queue>
#include <deque>
#include <string>
#include <vector>

#include <tuple>
#include <functional>
#include <memory>

#include "stop_words_table.hpp"
#include "vocabulary.hpp"
#include "logger.hpp"

typedef std::tuple<int, int, int> Triple;
typedef std::priority_queue<Triple, std::deque<Triple>, std::greater<Triple>> TriplesPQ;

/*typedef std::tuple<int, int, int, int> Quadruple;
typedef std::priority_queue<Quadruple, std::deque<Quadruple>, std::less<Quadruple>()> QuadruplesPQ;*/

class RunWriter;
class RunReader;

class Indexer{
public:
	//private:
		StopWordsTable *stop_words_table;
		Vocabulary *vocabulary;
		Logger *logger;

		size_t memory_limit;

		int runs_counter;
		char *runs_folder_path;
		std::queue<std::string> runs_filenames;

		void composeTriples(std::queue<std::string> &document_terms, int &document_id, TriplesPQ &ordered_triples, 
							RunWriter &run_writer, size_t &composition_memory);

		void intercalateRuns(std::vector<std::shared_ptr<RunReader>> &in_run_readers, RunWriter &out_run_writer);

	//public:
		Indexer(StopWordsTable *stop_words_table, Vocabulary *vocabulary, Logger *logger, size_t memory_limit, char *runs_folder_path);

		inline size_t getWorkingMemory(){
			return (memory_limit - vocabulary->getOccupiedSpace());
		}

		void composeRuns(std::queue<std::string> &htmls_files_list);

		void mergeRuns(int k);

		void composeIndex();
};

class RunWriter{
public:
	//private:
		std::ofstream file;
		std::filebuf *file_buffer;
		std::string file_path;
		size_t space_int;

	//public:
		RunWriter(){
			 file_buffer = file.rdbuf();
			 space_int = sizeof(int);
		}

		inline std::string getFilePath(){
			return file_path;
		}

		void open(std::string &file_path){
			this->file_path = file_path;
			file.open(file_path.c_str(), std::ios::binary);
			if(!file.is_open()) fprintf(stderr, "Error opening run_writer file '%s'", file_path.c_str());
		}

		inline void write(int &value){
			//file.write(reinterpret_cast<char*>(&value), space_int);
			file << value << ' ';
		}

		inline void write(TriplesPQ::size_type value){
			//file.write(reinterpret_cast<char*>(&value), sizeof(value));
			file << value << ' ';
		}

		inline void dump(){
			file_buffer->pubsync();
		}

		inline void close(){
			file.close();
		}

		inline ~RunWriter(){
			file.close(); file_buffer->close();
		}
};

class RunReader{

	private:
		std::ifstream file;
		std::filebuf *file_buffer;
		std::string file_path;
		size_t space_int;

	public:
		RunReader(){
			 file_buffer = file.rdbuf();
			 space_int = sizeof(int);
		}

		inline std::string getFilePath(){
			return file_path;
		}

		void open(std::string &file_path){
			this->file_path = file_path;
			file.open(file_path.c_str(), std::ios::binary);
			if(!file.is_open()) fprintf(stderr, "Error opening run_reader file '%s'", file_path.c_str());
		}

		inline void read(int &value){
			file.read(reinterpret_cast<char*>(&value), space_int);
		}

		inline void read(TriplesPQ::size_type value){
			file.read(reinterpret_cast<char*>(&value), sizeof(value));
		}

		/*inline void dump(){
			file_buffer->pubsync();
		}*/

		inline void close(){
			file.close();
		}

		inline ~RunReader(){
			file.close(); file_buffer->close();
		}
};

#endif