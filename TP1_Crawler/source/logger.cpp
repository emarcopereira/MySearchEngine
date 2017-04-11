#include "logger.hpp"
#include <iostream>

using namespace std;

Logger::Logger(int thread_id, const char *folder_path, const char *file_name, const size_t &buffer_size, chrono::steady_clock::duration start_time){
	this->start_time = start_time;
	this->thread_id = thread_id;
	this->dumper = new Dumper(folder_path, file_name, buffer_size);
	this->transaction_counter = 0;
}

void Logger::close(){
	this->dumper->close();
}

long long Logger::getRelativeTime(){
	std::chrono::steady_clock::duration current_time = chrono::steady_clock::now().time_since_epoch();
	return chrono::duration_cast<chrono::microseconds>(current_time - this->start_time).count();
}

/* TYPE: thread_execution */
size_t Logger::register_threadExecution_begin(){
	long long time = this->getRelativeTime();
	/* Format: [time] [transaction_id] thread_execution begin [thread_id]*/
	sprintf(this->message, "%lld %lu thread_execution begin %d\n", time, this->transaction_counter, this->thread_id);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_threadExecution_end(size_t transaction_id){
	long long time = this->getRelativeTime();
	/* Format: [time] [transaction_id] thread_execution end*/
	sprintf(this->message, "%lld %lu thread_execution end\n", time, transaction_id);
	this->dumper->write(this->message);
}

/* TYPE: write_disk */
size_t Logger::register_writeOnDisk_begin(const char *agent){
	long long time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] write_disk begin [agent]*/
	sprintf(this->message, "%lld %lu write_disk begin %s\n", time, this->transaction_counter, agent);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_writeOnDisk_end(size_t transaction_id, int quant_lines){
	long long time = this->getRelativeTime();
	/* Format: [time] [transaction_id] write_disk end [quant_lines]*/
	sprintf(this->message, "%lld %lu write_disk end %d\n", time, transaction_id, quant_lines);
	this->dumper->write(this->message);
}

/* TYPE: download_page */
size_t Logger::register_downloadPage_begin(const char *domain, const char *url){
	long long time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] download_page begin [domain] [url]*/
	sprintf(this->message, "%lld %lu download_page begin %s %s\n", time, this->transaction_counter, domain, url);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_downloadPage_end(size_t transaction_id, size_t page_size){
	long long time = this->getRelativeTime();
	/* Format: [time] [transaction_id] download_page end [page_size]*/
	sprintf(this->message, "%lld %lu download_page end %lu\n", time, transaction_id, page_size);
	this->dumper->write(this->message);
}

/* TYPE: download_blocked */
void Logger::register_downloadBlocked(const char *domain, int remain_time, const char *url){
	long long time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] download_blocked unique [domain] [remain_domain_time] [url]*/
	sprintf(this->message, "%lld %lu download_blocked unique %s %d %s\n", time, this->transaction_counter, domain, remain_time, url);
	this->dumper->write(this->message);
}

/* TYPE: wait_cz */
size_t Logger::register_waitCZ_begin(){
	long long time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] wait_cz begin [domain] [url]*/
	sprintf(this->message, "%lld %lu wait_cz begin\n", time, this->transaction_counter);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_waitCZ_end(size_t transaction_id){
	long long time = this->getRelativeTime();
	/* Format: [time] [transaction_id] wait_cz end*/
	sprintf(this->message, "%lld %lu wait_cz end\n", time, transaction_id);
	this->dumper->write(this->message);
}

/* TYPE: manage_cz */
size_t Logger::register_manageCZ_begin(){
	long long time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] manage_cz begin [domain] [url]*/
	sprintf(this->message, "%lld %lu manage_cz begin\n", time, this->transaction_counter);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_manageCZ_end(size_t transaction_id){
	long long time = this->getRelativeTime();
	/* Format: [time] [transaction_id] manage_cz end*/
	sprintf(this->message, "%lld %lu manage_cz end\n", time, transaction_id);
	this->dumper->write(this->message);
}