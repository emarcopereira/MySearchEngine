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
	this->logger->file << "\t[Composition_Memory: " << composition_memory << " Limit: " << this->getWorkingMemory() << "]\n";
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
			run_writer.write(value1); run_writer.write(value2); run_writer.write(value3); run_writer.file << "\n";
			ordered_triples.pop();
		}
		/* Closing current run */
		run_writer.dump();
		run_writer.close();
	}
	this->logger->file << "END Ordered Runs Compositon\n";
}

void Indexer::composeTriples(queue<string> &document_terms, int &document_id, TriplesPQ &ordered_triples,
							 RunWriter &run_writer, size_t &composition_memory){
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
	if(k < 2) return;
	int total_intercalations = 0;
	int total_levels = 0;

	this->logger->file << "BEGIN Runs Merging\n";
	while(this->runs_filenames.size() > 1){
		int n_runs_level = this->runs_filenames.size();
		int n_intercalations = ceil(float(n_runs_level)/float(k));

		this->logger->file << "\tLevel " << total_levels << "\n";

		/* Execute current level of intercalations (tree) */
		queue<string> intermediate_runs_filenames;
		for(int inter = 0; inter < n_intercalations; inter++){
			/* Skiping in case of only run */
			if(this->runs_filenames.size() == 1){
				intermediate_runs_filenames.push(this->runs_filenames.front());
				this->logger->file << "Passing Run '" << this->runs_filenames.front() << "' forward.\n";
				this->runs_filenames.pop();
				break;
			}

			/* Opening New Intermediate Run */
			RunWriter run_writer;
			string run_filename = runs_folder_path + string("/run_int_") + to_string(this->runs_counter++);
			run_writer.open(run_filename);
			intermediate_runs_filenames.push(run_filename);
			this->logger->file << "Creating Intermediate Run " << this->runs_counter-1 << ": '" << run_filename << "'\n";

			/* Opening Runs to be Intercalated */
			vector<shared_ptr<RunReader>> run_readers;
			for(int i = 0; (i < k) && (this->runs_filenames.size() > 0); i++){
				RunReader *run_reader = new RunReader();
				run_reader->open(this->runs_filenames.front());
				this->logger->file << "Opening Run '" << this->runs_filenames.front() << "'\n";
				this->runs_filenames.pop();
				run_readers.push_back(shared_ptr<RunReader>(run_reader));
			}
			//this->logger->dump();

			/* Intercalating Runs */
			this->intercalateRuns(run_readers, run_writer);

			run_writer.close();
		}
		this->runs_filenames = intermediate_runs_filenames;	

		total_intercalations += n_intercalations;
		total_levels++;
	}
	this->logger->file << "END Runs Merging\n";
	this->logger->file << "Total of intercalations: " << total_intercalations << "\n";
	this->logger->file << "Total of intercalation levels: " << total_levels << "\n";
}

typedef pair<Triple, int> IntercalationPQItem;
bool compare(IntercalationPQItem &pqi1, IntercalationPQItem &pqi2){ return pqi1.first > pqi2.first; }
typedef priority_queue<IntercalationPQItem, deque<IntercalationPQItem>, decltype(&compare)> IntercalationPQ;
void Indexer::intercalateRuns(vector<shared_ptr<RunReader>> &in_run_readers, RunWriter &out_run_writer){
	IntercalationPQ interPQ(&compare);
	int value1, value2, value3;
	shared_ptr<RunReader> run_reader_ptr;
	int n_in_runs = in_run_readers.size();

	vector<size_t> in_run_counters(n_in_runs, 0);
	in_run_counters.shrink_to_fit();

	/* Inserting First Triples */
	TriplesPQ::size_type n_tuples = 0;
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
	out_run_writer.write(total_tuples);
	while(!interPQ.empty()){
		run_index = interPQ.top().second;
		tie(value1, value2, value3) = interPQ.top().first;
		interPQ.pop();

		/* Writes triple */
		out_run_writer.write(value1); out_run_writer.write(value2); out_run_writer.write(value3); out_run_writer.file << '\n';
		if(in_run_counters[run_index] > 0){
			/* Reads new triple */
			run_reader_ptr = in_run_readers[run_index];
			run_reader_ptr->read(value1); run_reader_ptr->read(value2); run_reader_ptr->read(value3);
			in_run_counters[run_index]--;
			/* Adds triple to priority queue */
			interPQ.push( make_pair( make_tuple(value1, value2, value3), run_index) );
		}
	}
}

//unordered_map<int, int> term_frequencies;
/* //Counts term occurence 
if(not term_frequencies.emplace(term_id, 1).second)
	term_frequencies[term_id]++;
else // Data added to memory: 2*4(int) bytes 
	composition_memory += 8; */
void Indexer::composeIndex(string &output_file_path){
	RunReader run_reader;
	run_reader.open(this->runs_filenames.front());

	RunWriter run_writer;
	run_writer.open(output_file_path);

	TriplesPQ::size_type n_tuples = 0;
	run_reader.read(n_tuples);
	run_writer.write(n_tuples);

	int value1, value2, value3;
	int value1b, value2b, value3b;

	queue<int> positions;
	int frequency;

	run_reader.read(value1); run_reader.read(value2);
	long n_tuples_int = n_tuples;
	while(n_tuples_int > 0){
		do{	
			run_reader.read(value3b);
			positions.push(value3b); n_tuples_int--;
			run_reader.read(value1b); run_reader.read(value2b);
		}while(value1b == value1 && value2b == value2 && n_tuples_int);

		frequency = positions.size();
		while(!positions.empty()){
			run_writer.write(value1); run_writer.write(value2); run_writer.write(frequency); run_writer.write(positions.front()); run_writer.file << '\n';
			positions.pop();
		}

		value1 = value1b; value2 = value2b;
	}

	run_reader.close(); run_writer.close();
}