#ifndef _crawler_hpp
#define _crawler_hpp

#include <deque>
#include <unordered_map>
#include <string>
#include <memory>
#include <shared_mutex>

#include "spider.hpp"
#include "project_definitions.hpp"

/* Domains Structures */
typedef struct {
	int time_last_access;
	size_t domain_id;
} DomainCarrier;

/* Url Queues Structures */
class UrlLevelQueueCarrier{
	std::deque<UrlCarrier> *url_queue;
	std::shared_mutex url_queue_mutex;

	UrlLevelQueueCarrier(){
		url_queue = new std::deque<UrlCarrier>();
	}
	~UrlLevelQueueCarrier(){
		delete url_queue;
	}
};

/* Url Hashes Structures */
class UrlLevelHashCarrier{
	std::unordered_map<std::string, bool> *url_hash;
	std::shared_mutex url_hash_mutex;

	UrlLevelHashCarrier(){
		url_hash = new std::unordered_map<std::string, bool>();
	}
	~UrlLevelHashCarrier(){
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

		/* Domain Times Table */
		std::unordered_map<size_t, std::chrono::steady_clock::time_point> domains_times_hash;
		std::shared_mutex domain_hash_mutex;

		/* Urls Queues Array */
		std::array<UrlLevelQueueCarrier, MAX_URL_LEVEL> urls_queues;
		std::shared_mutex urls_queues_mutex;

	public:

		Crawler(char *pages_folder_path, char *logs_folder_path, char *stats_file_path, int buffer_pages_size, int buffer_iUrls_size, int buffer_oUrls_size, int buffer_log_size);

		~Crawler();

		void ExecuteCrawling();

		void Initialize(std::vector<std::string> &seeds);

		void StartCrawling(int n_threads);

		void StopCrawling();

		void ForceStopCrawling();

		void DoBackup();

		void LoadBackup();
};

#endif