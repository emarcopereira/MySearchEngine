#include "logger.hpp"

using namespace std;

Logger::Logger(int &thread_id, const char *folder_path, const char *file_name, const size_t &buffer_size){
	this->thread_id = thread_id;
	this->dumper = new Dumper(folder_path, file_name, buffer_size);
	this->transaction_counter = 0;
}

void Logger::close(){
	this->dumper.close();
}

int Logger::getRelativeTime(){
	std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
	return chrono::duration_cast<chrono::milliseconds>(current_time - this->start_time).count();
}

/* TYPE: thread_execution */
size_t Logger::register_threadExecution_begin(){
	int time = this->getRelativeTime();
	/* Format: [time] [transaction_id] thread_execution begin [thread_id]*/
	sprintf(this->message, "%d %lu thread_execution begin %d", time, this->transaction_counter, this->thread_id);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void register_threadExecution_end(size_t transaction_id){
	int time = this->getRelativeTime();
	/* Format: [time] [transaction_id] thread_execution end*/
	sprintf(this->message, "%d %lu thread_execution end", time, transaction_id);
	this->dumper->write(this->message);
}

/* TYPE: write_disk */
size_t register_writeOnDisk_begin(char *agent){
	int time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] write_disk begin [agent]*/
	sprintf(this->message, "%d %lu write_disk begin %s", time, this->transaction_counter, agent);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void register_writeOnDisk_end(size_t transaction_id, int quant_lines){
	int time = this->getRelativeTime();
	/* Format: [time] [transaction_id] write_disk end [quant_lines]*/
	sprintf(this->message, "%d %lu write_disk end %d", time, transaction_id, quant_lines);
	this->dumper->write(this->message);
}

/* TYPE: download_page */
size_t register_downloadPage_begin(const char *domain, const char *url){
	int time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] download_page begin [domain] [url]*/
	sprintf(this->message, "%d %lu download_page begin %s %s", time, this->transaction_counter, domain, url);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void register_downloadPage_end(size_t transaction_id, size_t page_size){
	int time = this->getRelativeTime();
	/* Format: [time] [transaction_id] download_page end [page_size]*/
	sprintf(this->message, "%d %lu download_page end %d", time, transaction_id, page_size);
	this->dumper->write(this->message);
}

/* TYPE: download_blocked */
void register_downloadBlocked(const char *domain, int remain_time, const char *url){
	int time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] download_blocked unique [domain] [remain_domain_time] [url]*/
	sprintf(this->message, "%d %lu download_blocked unique %s %d %s", time, this->transaction_counter, domain, remain_time, url);
	this->dumper->write(this->message);
}

/* TYPE: wait_cz */
size_t register_waitCZ_begin(){
	int time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] wait_cz begin [domain] [url]*/
	sprintf(this->message, "%d %lu wait_cz begin", time, this->transaction_counter);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void register_waitCZ_end(size_t transaction_id){
	int time = this->getRelativeTime();
	/* Format: [time] [transaction_id] wait_cz end*/
	sprintf(this->message, "%d %lu wait_cz end", time, transaction_id);
	this->dumper->write(this->message);
}

/* TYPE: manage_cz */
size_t register_manageCZ_begin(){
	int time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] manage_cz begin [domain] [url]*/
	sprintf(this->message, "%d %lu manage_cz begin", time, this->transaction_counter);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void register_manageCZ_end(size_t transaction_id){
	int time = this->getRelativeTime();
	/* Format: [time] [transaction_id] manage_cz end*/
	sprintf(this->message, "%d %lu manage_cz end", time, transaction_id);
	this->dumper->write(this->message);
}