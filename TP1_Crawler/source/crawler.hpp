#ifndef _crawler_hpp
#define _crawler_hpp

#include <deque>
#include <unordered_map>
#include <vector>
#include <list>
#include <utility>

#include <string>
#include <chrono>
#include <memory>

#include <thread>
#include <shared_mutex>
#include <mutex>
#include <atomic>

#include "spider.hpp"
#include "logger.hpp"
#include "monitor.hpp"
#include "project_definitions.hpp"

/* Url Levels Structures */
class UrlLevelCarrier{

	public:
		std::deque<UrlCarrier> *url_queue;
		std::unordered_map<size_t, bool> *url_hash;
		std::shared_mutex *url_level_mutex;

		UrlLevelCarrier(){
			url_queue = new std::deque<UrlCarrier>();
			url_hash = new std::unordered_map<size_t, bool>();
			url_level_mutex = new std::shared_mutex();
		}
		/*UrlLevelCarrier(const &UrlLevelCarrier ul) : 
			url_queue(ul.url_queue), url_hash(ul.url_hash), url_level_mutex(ul.url_level_mutex){
		}*/
		~UrlLevelCarrier(){}
};


class Crawler{
	
	public://private:

		/* Parameters */
		char *pages_folder_path;
		char *logs_folder_path;
		char *stats_file_path;
		char *backup_folder_path;
		int buffer_pages_size;
		int buffer_iUrls_size;
		int buffer_oUrls_size;
		int buffer_log_size;

		/* Logger */
		Logger *logger;
		size_t log_start_id;

		/* Monitor */
		Monitor *monitor;

		/* Domain Times Table */
		std::unordered_map<size_t, std::chrono::steady_clock::time_point> domain_times_hash;
		std::mutex domain_hash_mutex;

		/* Url Levels Structures */
		std::vector<UrlLevelCarrier> urls_levels;

		/* Threads Control Structures */
		bool stop_command;
		std::shared_mutex stop_command_mutex;
		std::list<std::thread> threads;

	//public:

		Crawler(char *pages_folder_path, char *logs_folder_path, char *backup_folder_path, char *stats_file_path, int buffer_pages_size,
				 int buffer_iUrls_size, int buffer_oUrls_size, int buffer_log_size);

		~Crawler();

		void Initialize(std::vector<std::string> &seeds);

		void StartCrawling(int n_threads);

		void StopCrawling();

		void ExecuteCrawling(int thread_id);

		void ForceStopCrawling();

		void DoBackup();

		void LoadBackup();
};

#endif