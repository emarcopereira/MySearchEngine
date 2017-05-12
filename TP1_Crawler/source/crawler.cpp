#include "crawler.hpp"

using namespace std;

Crawler::Crawler(char *pages_folder_path, char *logs_folder_path, char *backup_folder_path, char *stats_file_path, int buffer_pages_size,
				 int buffer_iUrls_size, int buffer_oUrls_size, int buffer_log_size){
	/* Parameters */
	this->pages_folder_path = pages_folder_path;
	this->logs_folder_path = logs_folder_path;
	this->stats_file_path = stats_file_path;
	this->buffer_pages_size = buffer_pages_size;
	this->buffer_iUrls_size = buffer_iUrls_size;
	this->buffer_oUrls_size = buffer_oUrls_size;
	this->buffer_log_size = buffer_log_size;
	this->backup_folder_path = backup_folder_path;

	/* Constructing Log Structures */
	this->logger = new Logger(-2, this->logs_folder_path, "crawler.log", this->buffer_log_size, chrono::steady_clock::now());
	this->log_start_id = this->logger->register_threadExecution_begin();

	/* Constructing Url Structures */
	for(int i=0; i<MAX_URL_LEVEL; i++)
		this->urls_levels.emplace_back(UrlLevelCarrier());
	this->urls_levels.shrink_to_fit();

	/* Constructing Monitor Structures */
	this->monitor = new Monitor(this->stats_file_path, chrono::steady_clock::now());
}

Crawler::~Crawler(){
	if(this->monitor != NULL)
		delete this->monitor;

	this->logger->register_threadExecution_end(this->log_start_id);

	if(this->logger != NULL)
		delete this->logger;

	/* Deallocating Url Structures */
	for(int i=0; i<MAX_URL_LEVEL; i++){
		delete this->urls_levels[i].url_queue;
		delete this->urls_levels[i].url_hash;
		delete this->urls_levels[i].url_level_mutex;
	}
}

void Crawler::Initialize(vector<string> &seeds){
	int initializer_id = -1;
	chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
	Logger logger(initializer_id, this->logs_folder_path, "initializer.log", buffer_log_size, start_time);
	Dumper dumper(this->pages_folder_path, "initializer.dump", this->buffer_pages_size);
	Spider spider(this->buffer_iUrls_size, this->buffer_oUrls_size, &logger, &dumper);
	Url *url_manager = spider.url_manager;

	/* Adding counter for initializer downloaded pages */
	this->monitor->include_value(initializer_id, 0);

	/* Launches Monitor */
    monitor->start_monitoring(chrono::minutes(MONITORING_TIME_INTERVAL));

	CkString canonized_url;
	size_t domain_id, url_id;
	CkString aux_url;
	int n_levels;

	/* Collecting seed urls */
	int size = seeds.size();
	for(int i=0; i<size; i++){
		/* Removing in case of failed canonization*/
        if(!url_manager->getCleanUrl(seeds[i].c_str(), aux_url))
            continue;

        string canonized_url = aux_url.getString();
        canonized_url.shrink_to_fit();

        n_levels = url_manager->getNumberLevels(canonized_url);

        /* Removing urls with more levels than permited */
        if(n_levels > MAX_URL_LEVEL)
            continue;

        string domain = url_manager->getDomain(aux_url.getString());
        domain.shrink_to_fit();

	    domain_id = url_manager->getStringHash(domain);
	    url_id = url_manager->getStringHash(canonized_url);
	    fprintf(stderr, "Added SeedUrl: %s Domain: %s NLevels: %d\n", canonized_url.c_str(), domain.c_str(), n_levels);

	  	/* Trying to insert on Shared Structures */
	    auto answer2 = this->urls_levels[n_levels-1].url_hash->emplace(url_id, true);
	  	if(answer2.second == false) /* IF equal url already exists */
	  		continue;

	  	auto answer = this->domain_times_hash.emplace(domain_id, chrono::steady_clock::now());
	  	if(answer.second == false) /* IF equal domain already exists */
	  		continue;

	  	spider.collect(UrlCarrier(canonized_url, url_id, domain, domain_id, n_levels));
  	}

  	/* Adding counter for initializer downloaded pages */
	this->monitor->update_value(initializer_id, spider.page_count);

  	/* Adding collected urls to queues */
  	for(int level=2; level<=MAX_URL_LEVEL; level++){
  		UrlLevelCarrier &url_level = this->urls_levels[level-1];

  		queue<UrlCarrier> &buffer_level_queue = spider.buffer_inURLs[level-1];
  		size = buffer_level_queue.size();
  		for(int i=0; i<size; i++){
  			UrlCarrier &url_carrier = buffer_level_queue.front();

  			auto answer = url_level.url_hash->emplace(url_carrier.url_id, true);
  			if(answer.second == true) /* IF there is no equal url */
  				url_level.url_queue->push_back(url_carrier);

  			buffer_level_queue.pop();
  			spider.buffer_inURLs_count--;
  		} 
  	}

  	/* Closing objects */
	spider.close();
}

void Crawler::StartCrawling(int n_threads){
	/* Initializing thread control structures */
	this->stop_command = false;

	/* Adding counter for threads downloaded pages */
	for(int i=0; i < n_threads; i++)
		this->monitor->include_value(i, 0);
	
	fprintf(stderr, "\n[Crawler] Threads starting...");
   	this->logger->register_simpleLog("\n[Crawler] Threads starting...");

   	/* Launches Threads */
	for(int i=0; i < n_threads; i++)
    	this->threads.push_back(thread(&Crawler::ExecuteCrawling, this, i));

    /* Sincronize Threads */
  	for(list<thread>::iterator it = this->threads.begin(); it != this->threads.end(); it++)
   		it->join();
}

void Crawler::ExecuteCrawling(int thread_id){
	chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
	string log_file_name = "thread_" + to_string(thread_id) + ".log";
	string dump_file_name = "thread_" + to_string(thread_id) + ".dump";

	Logger logger(thread_id, this->logs_folder_path, log_file_name.c_str(), this->buffer_pages_size, start_time);
	Dumper dumper(this->pages_folder_path, dump_file_name.c_str(), this->buffer_pages_size);
	Spider spider(this->buffer_iUrls_size, this->buffer_oUrls_size, &logger, &dumper);

	bool stop_command_activated;
	bool couldnt_complete_outBuffer = false;
	while(true){
	 	{shared_lock<shared_mutex> lock(this->stop_command_mutex);
	 		stop_command_activated = this->stop_command;
	 	}
	 	if(stop_command_activated) break;

	 	if(!spider.buffer_outURLs.empty()){
	 		/* Collecting a page */
	 		UrlCarrier &url_carrier = spider.buffer_outURLs.front();
	 		spider.collect(url_carrier);
			spider.buffer_outURLs.pop();
	 		monitor->update_value(thread_id, spider.page_count);
	 	}
	 	else{ /* Retrieving urls from queues */
			int cont_unseen; size_t domain_id;
			int cont_to_be_retrieved = spider.buffer_oUrls_size;
			chrono::steady_clock::time_point current_point;

			int level;
			couldnt_complete_outBuffer = false;
			for(level=2; level<=MAX_URL_LEVEL; level++){
				UrlLevelCarrier &url_level = this->urls_levels[level-1];
				deque<UrlCarrier> *url_queue = url_level.url_queue;

				{lock_guard<shared_mutex> lock(*(url_level.url_level_mutex)); /* Locking Url Level */
					cont_unseen = url_queue->size();

					while(cont_to_be_retrieved && cont_unseen--){
						UrlCarrier &url_carrier = url_queue->front();
						domain_id = url_carrier.domain_id;
						current_point = chrono::steady_clock::now();

						/* Verifies domain existence/Include doamain in times hash */
						this->domain_hash_mutex.lock();
							auto answer = this->domain_times_hash.emplace(domain_id, current_point);
						this->domain_hash_mutex.unlock();

						/* Verifies IF is the first domain access */
						if(answer.second == true){
							cont_to_be_retrieved--;

							/* Add url to the out_buffer */
							spider.buffer_outURLs.push(url_carrier);
						}
						/* Verifies time access rule (politeness) */
						else if(chrono::duration_cast<chrono::seconds>(current_point - answer.first->second).count() >= DOMAIN_ACCESS_TIME){
							/* Update domain access time */
							this->domain_hash_mutex.lock();
								this->domain_times_hash[domain_id] = current_point;
							this->domain_hash_mutex.unlock();

							cont_to_be_retrieved--;

							/* Add url to the out_buffer */
							spider.buffer_outURLs.push(url_carrier);
						}
						/* Move url to end of queue */
						else url_queue->push_back(url_carrier);

						url_queue->pop_front();
					}

				} /* Unlocking Url Level */
				/* Doesn't look in next levels if the search is done */
				if(cont_to_be_retrieved == 0) break;	
			}
			if(spider.buffer_oUrls_size-cont_to_be_retrieved > 0)
				logger.register_getUrls(spider.buffer_oUrls_size - cont_to_be_retrieved, level);
			if(level >= MAX_URL_LEVEL) couldnt_complete_outBuffer = true;
		}

		if(spider.buffer_inURLs_count > spider.buffer_iUrls_size || (couldnt_complete_outBuffer && spider.buffer_outURLs.empty())){
			if(spider.buffer_inURLs_count == 0) continue;

			/* Transfering urls to queues */
			int size;//int added_urls = 0;

			logger.register_moveUrls(spider.buffer_inURLs_count);
			for(int level=2; level<=MAX_URL_LEVEL; level++){
		  		UrlLevelCarrier &url_level = this->urls_levels[level-1];
		  		queue<UrlCarrier> &buffer_level_queue = spider.buffer_inURLs[level-1];
		  		size = buffer_level_queue.size();
				
				{lock_guard<shared_mutex> lock(*(url_level.url_level_mutex)); /* Locking Url Level */
			  		for(int i=0; i<size; i++){
			  			UrlCarrier &url_carrier = buffer_level_queue.front();
				  		auto answer = url_level.url_hash->emplace(url_carrier.url_id, true);
				  		if(answer.second == true) /* IF there is no equal url */
				  			url_level.url_queue->push_back(url_carrier);

			  			buffer_level_queue.pop();
			  			spider.buffer_inURLs_count--;
			  		}
		  		} /* Unlocking Url Level */
		  	}
			//logger->register_addUrls();
		}
	}

	/* Closing objects */
	spider.close();
}

void Crawler::DoBackup(){
	//this->logger->register_simpleLog("\n[Crawler] Backing up...");
	//fprintf(stderr, "\n[Crawler] Backing up...");

	/* Urls Queues (url, domain, domain_id, n_levels) */

	/* Urls Hash Table (url_hash) */

	//this->logger->register_simpleLog("\n[Crawler] Backup Finished.");
	//fprintf(stderr, "\n[Crawler] Backup Finished.");
}

void Crawler::LoadBackup(){

}

void Crawler::StopCrawling(){

   	/* Stops Monitor Thread */
   	monitor->stop_monitoring();

	fprintf(stderr, "\n[Crawler] Threads finished crawling.");
   	this->logger->register_simpleLog("\n[Crawler] Threads finished crawling.");

	this->DoBackup();
}

void Crawler::ForceStopCrawling(){
	fprintf(stderr, "\n[Crawler] Initiating Forced Stop.");
	this->logger->register_simpleLog("\n[Crawler] Initiating Forced Stop.");

	/* Giving stop command to threads */
	{lock_guard<shared_mutex> lock(this->stop_command_mutex);
		this->stop_command = true;
	}
}