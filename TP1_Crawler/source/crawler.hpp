#ifndef _crawler_hpp
#define _crawler_hpp

#include <queue>
#include <deque>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <memory>

/* Domains Queue's Structures */
typedef struct {
	int time_last_access;
	size_t domain_id;
} DomainsQueueItem;

struct DomainsQueueItemCompare{
	bool operator()(const DomainsQueueItem &i1, const DomainsQueueItem &i2) const{
		return i1.time_last_access > i2.time_last_access;
	}
};

/* Urls Queue's Structures*/
typedef struct{
	int n_levels;
	std::shared_ptr<std::string> *url_sp;
} UrlsQueueItem;

struct UrlsQueueItemCompare{
	bool operator()(const UrlsQueueItem &i1, const UrlsQueueItem &i2) const{
		return i1.n_levels > i2.n_levels;
	}
};

typedef std::priority_queue<UrlsQueueItem, std::deque<UrlsQueueItem>, UrlsQueueItemCompare> UrlsQueue;

/* Visited Urls Hash's Structures*/
typedef std::unordered_map<std::string, size_t> VisitedUrlsHash;

/* Domains Info Hash's Structures */
typedef struct{
	size_t domain_id;
	int time_last_access;

	std::shared_ptr<UrlsQueue> urls_queue_sp;
	std::mutex urls_queue_mutex;

	std::shared_ptr<VisitedUrlsHash> visited_urls_hash_sp;
	std::mutex visited_urls_hash_mutex;

} DomainsInfoItem;

class Crawler{
	
	private:

		/* Parameters */
		char *pages_folder_path;
		char *logs_folder_path;
		char *stats_file_path;
		int buffer_pages_size;
		int buffer_iUrls_size;
		int buffer_oUrls_size;

		/* Domains Queue */
		std::priority_queue<DomainsQueueItem, std::deque<DomainsQueueItem>, DomainsQueueItemCompare> domains_queue;
		std::mutex domains_queue_mutex;
		bool is_domains_queue_sorted;

		/* Domains Info Hash */
		std::unordered_map<size_t, DomainsInfoItem> domains_info_hash;
		std::mutex domains_info_mutex;

		void ExecuteCrawling();

	public:

		Crawler(char *pages_folder_path, char *logs_folder_path, char *stats_file_path, int buffer_pages_size, int buffer_iUrls_size, int buffer_oUrls_size);

		~Crawler();

		void Initialize(std::vector<std::string> &seeds);

		void StartCrawling(int n_threads);

		void StopCrawling();

		void ForceStopCrawling();
};

#endif