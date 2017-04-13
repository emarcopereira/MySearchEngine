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
		int buffer_pages_size;
		int buffer_iUrls_size;
		int buffer_oUrls_size;

		/* Domain Index Table */
		std::unordered_map<std::string, size_t> domains_index;
		std::shared_mutex domains_index_mutex;

		/* Domain Times Table */
		std::unordered_map<std::string, size_t> domains_hash;
		std::shared_mutex domain_hash_mutex;

		/* Urls Queues Array */
		std::array<UrlLevelQueueCarrier, MAX_URL_LEVEL> urls_queues;
		std::shared_mutex urls_queues_mutex;

		/* Urls Hashes Array */
		std::vector<UrlLevelHashCarrier, MAX_URL_LEVEL> urls_hashes;
		std::shared_mutex urls_hashes_mutex;

	public:

		Crawler(char *pages_folder_path, char *logs_folder_path, char *stats_file_path, int buffer_pages_size, int buffer_iUrls_size, int buffer_oUrls_size);

		~Crawler();

		void ExecuteCrawling();

		void Initialize(std::vector<std::string> &seeds);

		void StartCrawling(int n_threads);

		void StopCrawling();

		void ForceStopCrawling();
};

#endif