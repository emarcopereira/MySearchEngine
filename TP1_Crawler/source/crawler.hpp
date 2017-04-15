#ifndef _crawler_hpp
#define _crawler_hpp

#include <deque>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <shared_mutex>
#include <chrono>

#include "spider.hpp"
#include "logger.hpp"
#include "project_definitions.hpp"

/* Url Levels Structures */
class UrlLevelCarrier{

	public:
		std::deque<UrlCarrier> *url_queue;
		std::unordered_map<std::string, bool> *url_hash;
		std::shared_mutex url_level_mutex;

		UrlLevelCarrier(){
			url_queue = new std::deque<UrlCarrier>();
			url_hash = new std::unordered_map<std::string, bool>();
		}
		~UrlLevelCarrier(){
			delete url_queue;
			delete url_hash;
		}
};


class Crawler{
	
	private:

		/* Parameters */
		char *pages_folder_path;
		char *logs_folder_path;
		char *stats_file_path;
		char *backup_folder_path;
		int buffer_pages_size;
		int buffer_iUrls_size;
		int buffer_oUrls_size;
		int buffer_log_size;

		bool stop_command;
		Logger *logger;
		size_t log_start_id;

		/* Urls Queues Array */
		std::vector<UrlLevelCarrier> urls_levels;

		/* Domain Times Table */
		std::unordered_map<size_t, std::chrono::steady_clock::time_point> *domain_times_hash;
		std::shared_mutex domain_hash_mutex;

	public:

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

		void SaveStatistics();
};

#endif