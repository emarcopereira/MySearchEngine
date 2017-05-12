#include "monitor.hpp"

using namespace std;

Monitor::Monitor(const char *stats_file_path, chrono::steady_clock::time_point start_time){
	this->stats_file_path = stats_file_path;
	this->start_time = start_time;

	this->stats_file = fopen(this->stats_file_path, "w");
	if(stats_file == NULL){
		fprintf(stderr, "\n\tError opening output file '%s'", this->stats_file_path);
		exit(EXIT_FAILURE);
	}

	this->stop_command = false;
}

void Monitor::write_values(){
	chrono::steady_clock::time_point current_time = chrono::steady_clock::now();
	fprintf(this->stats_file, "%lu ", chrono::duration_cast<chrono::seconds>(current_time - this->start_time).count());

	int sum = 0;
	{shared_lock<shared_mutex> lock(this->values_mutex);
		for(auto it = this->values.begin(); it != this->values.end(); ++it){
			sum += (*it).second;
			fprintf(this->stats_file, " %d: %d", (*it).first, (*it).second);
		}
	}
	fprintf(this->stats_file, " total: %d\n", sum);
	fflush(this->stats_file);
}

Monitor::~Monitor(){
	this->write_values();

	fclose(this->stats_file);
}
