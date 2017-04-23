#include "logger.hpp"
#include <iostream>

using namespace std;

Logger::Logger(int thread_id, const char *folder_path, const char *file_name, const size_t buffer_size, chrono::steady_clock::time_point start_time){
	this->start_time = start_time;
	this->thread_id = thread_id;
	this->dumper = new Dumper(folder_path, file_name, buffer_size);
	this->transaction_counter = 0;
}

Logger::~Logger(){
	if(this->dumper != NULL)
		delete this->dumper;
}

size_t Logger::getRelativeTime(){
	chrono::steady_clock::time_point current_time = chrono::steady_clock::now();
	return chrono::duration_cast<chrono::milliseconds>(current_time - this->start_time).count();
}

/* TYPE: thread_execution */
size_t Logger::register_threadExecution_begin(){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] thread_execution begin [thread_id]*/
	sprintf(this->message, "%lu %lu thread_execution begin %d\n", time, this->transaction_counter, this->thread_id);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_threadExecution_end(size_t transaction_id){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] thread_execution end*/
	sprintf(this->message, "%lu %lu thread_execution end\n", time, transaction_id);
	this->dumper->write(this->message);
}

/* TYPE: write_disk */
size_t Logger::register_writeOnDisk_begin(const char *agent){
	size_t time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] write_disk begin [agent]*/
	sprintf(this->message, "%lu %lu write_disk begin %s\n", time, this->transaction_counter, agent);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_writeOnDisk_end(size_t transaction_id, int quant_lines){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] write_disk end [quant_lines]*/
	sprintf(this->message, "%lu %lu write_disk end %d\n", time, transaction_id, quant_lines);
	this->dumper->write(this->message);
}

/* TYPE: download_page */
size_t Logger::register_downloadPage_begin(const char *domain, size_t domain_id, const char *url, size_t url_id, int n_levels){
	size_t time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] download_page begin [domain] [domain_id] [url] [url_n_levels] */
	sprintf(this->message, "%lu %lu download_page begin %s %lu %s %lu %d\n", time, this->transaction_counter, domain, domain_id, url, url_id,n_levels);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_downloadPage_end(size_t transaction_id, bool sucess, size_t page_size){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] download_page end [sucess] [page_size]*/
	sprintf(this->message, "%lu %lu download_page end %d %lu\n", time, transaction_id, sucess, page_size);
	this->dumper->write(this->message);
}

/* TYPE: download_blocked */
/*void Logger::register_downloadBlocked(const char *domain, int remain_time, const char *url){
	size_t time = this->getRelativeTime();
	this->transaction_counter++;*/
	/* Format: [time] [transaction_id] download_blocked unique [domain] [remain_domain_time] [url]*/
	/*sprintf(this->message, "%lu %lu download_blocked unique %s %d %s\n", time, this->transaction_counter, domain, remain_time, url);
	this->dumper->write(this->message);
}*/

/* TYPE: wait_cz */
size_t Logger::register_waitCZ_begin(){
	size_t time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] wait_cz begin [domain] [url]*/
	sprintf(this->message, "%lu %lu wait_cz begin\n", time, this->transaction_counter);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_waitCZ_end(size_t transaction_id){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] wait_cz end*/
	sprintf(this->message, "%lu %lu wait_cz end\n", time, transaction_id);
	this->dumper->write(this->message);
}

/* TYPE: manage_cz */
size_t Logger::register_manageCZ_begin(){
	size_t time = this->getRelativeTime();
	this->transaction_counter++;
	/* Format: [time] [transaction_id] manage_cz begin [domain] [url]*/
	sprintf(this->message, "%lu %lu manage_cz begin\n", time, this->transaction_counter);
	this->dumper->write(this->message);
	return this->transaction_counter;
}
void Logger::register_manageCZ_end(size_t transaction_id){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] manage_cz end*/
	sprintf(this->message, "%lu %lu manage_cz end\n", time, transaction_id);
	this->dumper->write(this->message);
}

/* TYPE: simple_log */
void Logger::register_simpleLog(const char *message){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] simple_log [message]*/
	sprintf(this->message, "%lu %lu simple_log %s\n", time, this->transaction_counter, message);
	this->dumper->write(this->message);
}

/* TYPE: link_extraction */
void Logger::register_linkExtraction(int n_inbounds, int n_outbounds){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] link_extraction [n_inbounds] [n_outbounds] [total] */
	sprintf(this->message, "%lu %lu link_extraction %d %d %d\n", time, this->transaction_counter, n_inbounds, n_outbounds, n_inbounds+n_outbounds);
	this->dumper->write(this->message);
}

/* TYPE: move_urls */
void Logger::register_moveUrls(int n_urls){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] move_urls [n_urls] */
	sprintf(this->message, "%lu %lu move_urls %d\n", time, this->transaction_counter, n_urls);
	this->dumper->write(this->message);
}

/* TYPE: add_urls */
void Logger::register_addUrls(int n_urls){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] add_urls [n_urls] */
	sprintf(this->message, "%lu %lu add_urls %d\n", time, this->transaction_counter, n_urls);
	this->dumper->write(this->message);
}

/* TYPE: get_urls */
void Logger::register_getUrls(int n_urls, int last_level){
	size_t time = this->getRelativeTime();
	/* Format: [time] [transaction_id] get_urls [n_urls] */
	sprintf(this->message, "%lu %lu get_urls %d %d\n", time, this->transaction_counter, n_urls, last_level);
	this->dumper->write(this->message);
}