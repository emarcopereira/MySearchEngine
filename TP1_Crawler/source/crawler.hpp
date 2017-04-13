#ifndef _crawler_hpp
#define _crawler_hpp

#include <deque>
#include <unordered_map>
#include <string>
#include <shared_mutex>

/* Domains tructures */
typedef struct {
	int time_last_access;
	size_t domain_id;
} DomainCarrier;

/* Urls Structures*/
typedef struct {
	std::string *url;
	int n_levels;
	size_t domain_id;
} UrlCarrier;

class Crawler{
	
	private:

		/* Parameters */
		char *pages_folder_path;
		char *logs_folder_path;
		char *stats_file_path;
		int buffer_pages_size;
		int buffer_iUrls_size;
		int buffer_oUrls_size;

		/* Domain Index */
		unordered_map<std::string, size_t> domain_index;



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