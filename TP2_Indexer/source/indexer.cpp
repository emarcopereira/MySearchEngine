#include "indexer.hpp"

#include "parser.hpp"

#include <fstream>
#include <cstdio>

#include <unordered_map>
#include <vector>

#include <cmath>

using namespace std;

Indexer::Indexer(StopWordsTable *stop_words_table, Vocabulary *vocabulary, Logger *logger,
				 size_t memory_limit, char *runs_folder_path){
	this->stop_words_table = stop_words_table;
	this->vocabulary = vocabulary;
	this->logger = logger;

	this->memory_limit = memory_limit - stop_words_table->getOccupiedSpace();

	this->runs_counter = 0;
	this->runs_folder_path = runs_folder_path;
}

void Indexer::composeRuns(std::queue<std::string> &htmls_files_list){
	TriplesPQ ordered_triples;
	Parser parser;
	string file_content;

	int document_counter = 0;
	size_t composition_memory = 0;

	this->logger->file << "BEGIN Ordered Runs Compositon\n";

	RunWriter run_writer;
	string run_filename = runs_folder_path + string("/run_") + to_string(this->runs_counter++);
	run_writer.open(run_filename);
	this->runs_filenames.push(run_filename);
	this->logger->file <<"\t[Composition_Memory: " << composition_memory << " Limit: " << this->getWorkingMemory() << "]\n";
	this->logger->file << "\nNew Run openned: '" << run_filename << "'.\n";

	while(!htmls_files_list.empty()){
		string &html_filename = htmls_files_list.front();

		/* Reading HTMLs File */
		ifstream file(html_filename.c_str());
		getline(file, file_content);
		file_content.shrink_to_fit();
		if(file.fail()){
			fprintf(stderr, "ERROR READING FILE: '%s'\n", html_filename.c_str());
			file.close(); htmls_files_list.pop();
			continue;
		}
		file.close();
		htmls_files_list.pop();
		
		/* Separating HTMLs and processing them */
			//FILE *out_file = fopen("out3.txt", "w");
		size_t it2, it1 = file_content.find("|||", 0);
		for( ; ; ){
			it1 += 3;
			/* Delimits url */
			it2 = file_content.find("|", it1);
				//fprintf(stderr, "Ao pegar URL: it1:%lu it2:%lu diff:%lu\n", it1, it2, it2-it1);
			if(it2 == string::npos) break;
			/* Gets url */
			const string &url = file_content.substr(it1, it2-it1);
			this->logger->file << "Html " << document_counter << " reached: "<< url << "\n";
				//fprintf(stderr, "Url Size: %lu\n", url.size()); fprintf(stderr, "%s\n", url.c_str());

			it2 += 1;
			/* Delimits html */
			it1 = file_content.find("|||", it2);
				//fprintf(stderr, "Ao pegar HTML: it1:%lu it2:%lu diff:%lu\n", it2, it1, it1-it2);
			/* Gets html */
			const string &html = file_content.substr(it2, it1-it2);
				//printf("Html Size: %lu\n", html.size()); fprintf(out_file, "%s\n\n", html.c_str());

			/* Process html */
			queue<string> document_terms = parser.getTerms(html, this->stop_words_table);
			this->logger->file << "Terms Parsed: " << document_terms.size() << "\n";
			this->composeTriples(document_terms, document_counter, ordered_triples, run_writer, composition_memory);
			document_counter++;
		}
	}

	/* Dumping the left triples */
	int value1, value2, value3;
	if(composition_memory > 0){
		this->logger->file << ordered_triples.size() << " triples added to '" << run_writer.getFilePath() << "'\n";
		this->logger->file <<"\t[Composition_Memory: " << composition_memory << " Limit: " << this->getWorkingMemory() << "]\n";

		/* Dumping existent triples */
		run_writer.write(ordered_triples.size());
		while(!ordered_triples.empty()){
			tie(value1, value2, value3) = ordered_triples.top();
			run_writer.write(value1); run_writer.write(value2); run_writer.write(value3); //run_writer.file << "\n";
			ordered_triples.pop();
		}
		/* Closing current run */
		run_writer.dump();
		run_writer.close();
	}
	this->logger->file << "END Ordered Runs Compositon\n";
}

//unordered_map<int, int> term_frequencies;
/* //Counts term occurence 
if(not term_frequencies.emplace(term_id, 1).second)
	term_frequencies[term_id]++;
else // Data added to memory: 2*4(int) bytes 
	composition_memory += 8; */

void Indexer::composeTriples(queue<string> &document_terms, int &document_id, TriplesPQ &ordered_triples,
							 RunWriter &run_writer, size_t &composition_memory){
	//unordered_map<int, int> term_frequencies;
	int term_id, position_counter = 0;

	while(!document_terms.empty()){
		string &term = document_terms.front();

		/* Tryes to add term to vocabulary */
		term_id = this->vocabulary->addTerm(term);
		document_terms.pop();

		/* Saves term position */
		ordered_triples.emplace(make_tuple(term_id, document_id, position_counter));
		/* Data added to memory: 3*4(int) bytes */
		composition_memory += 12;

		position_counter++;

		if(composition_memory >= this->getWorkingMemory()){ /* Memory is full */
			this->logger->file << ordered_triples.size() << " triples added to '" << run_writer.getFilePath() << "'\n";
			this->logger->file <<"\t[Composition_Memory: " << composition_memory << " Limit: " << this->getWorkingMemory() << "]\n";

			/* Dumping existent triples */
			int value1, value2, value3;
			run_writer.write(ordered_triples.size());
			while(!ordered_triples.empty()){
				tie(value1, value2, value3) = ordered_triples.top();
				run_writer.write(value1); run_writer.write(value2); run_writer.write(value3);
				ordered_triples.pop();
			}
			/* Closing current run */
			run_writer.dump();
			run_writer.close();
			/* Opening new run */
			string run_filename = runs_folder_path + string("/run_") + to_string(this->runs_counter++);
			run_writer.open(run_filename);
			this->runs_filenames.push(run_filename);
			this->logger->file << "\nNew Run openned: '" << run_filename << "'.\n";

			composition_memory = 0;
		}
	}
}

void Indexer::mergeRuns(int k){
	int total_intercalations = 0;
	int total_levels = 0;

	while(this->runs_filenames.size() > 1){
		int n_runs_level = this->runs_filenames.size();
		int n_intercalations = ceil(float(n_runs_level)/float(k));

		/* Execute current level of intercalations (tree) */
		queue<string> intermediate_runs_filenames;
		for(int inter = 0; inter < n_intercalations; inter++){
			/* Opening New Intermediate Run */
			RunWriter run_writer;
			string run_filename = runs_folder_path + string("/run_int_") + to_string(this->runs_counter++);
			run_writer.open(run_filename);
			intermediate_runs_filenames.push(run_filename);

			/* Opening Runs to be Intercalated */
			vector<shared_ptr<RunReader>> run_readers;
			for(int i = 0; (i < k) && (this->runs_filenames.size() > 0); i++){
				RunReader *run_reader = new RunReader();
				run_reader->open(this->runs_filenames.front());
				this->runs_filenames.pop();
				run_readers.push_back(shared_ptr<RunReader>(run_reader));
			}

			//intercalate

			run_writer.close();
		}
		this->runs_filenames = intermediate_runs_filenames;	

		total_intercalations += n_intercalations;
		total_levels++;
	}
}

typedef pair<Triple, int> IntercalationPQItem;
bool compare(IntercalationPQItem &pqi1, IntercalationPQItem &pqi2){ return pqi1.first > pqi2.first; }
typedef priority_queue<IntercalationPQItem, deque<IntercalationPQItem>, decltype(&compare)> IntercalationPQ;
void Indexer::intercalateRuns(vector<shared_ptr<RunReader>> &in_run_readers, RunWriter &out_run_writer){
	IntercalationPQ interPQ;
	int value1, value2, value3;
	shared_ptr<RunReader> run_reader_ptr;
	int n_in_runs = in_run_readers.size();

	vector<size_t> in_run_counters(n_in_runs, 0);
	in_run_counters.shrink_to_fit();

	/* Inserting First Triples */
	TriplesPQ::size_type n_tuples;
	size_t total_tuples = 0;
	for(int i=0; i<n_in_runs; i++){
		run_reader_ptr = in_run_readers[i];

		run_reader_ptr->read(n_tuples);
		in_run_counters[i] = n_tuples;
		total_tuples += n_tuples;

		/* Reads triple */
		run_reader_ptr->read(value1); run_reader_ptr->read(value2); run_reader_ptr->read(value3);
		in_run_counters[i]--;
		/* Adds triple to priority queue */
		if(in_run_counters[i] > 0)
			interPQ.push( make_pair( make_tuple(value1, value2, value3), i) );
	}

	/* Intercalating */
	int run_index;
	while(!interPQ.empty()){
		run_index = interPQ.top().second;
		tie(value1, value2, value3) = interPQ.top().first;
		interPQ.pop();

		/* Writes triple */
		out_run_writer.write(value1); out_run_writer.write(value2); out_run_writer.write(value3);
		/* Reads new triple */
		run_reader_ptr->read(value1); run_reader_ptr->read(value2); run_reader_ptr->read(value3);
		in_run_counters[run_index]--;
		/* Adds triple to priority queue */
		if(in_run_counters[run_index] > 0)
			interPQ.push( make_pair( make_tuple(value1, value2, value3), run_index) );
	}
}
