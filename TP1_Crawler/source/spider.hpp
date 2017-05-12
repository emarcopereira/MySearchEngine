#ifndef _spider_hpp
#define _spider_hpp

#include <string>
#include <queue>
#include <memory>

#include "../chilkat/include/CkSpider.h"

#include "url.hpp"
#include "dumper.hpp"
#include "logger.hpp"

/* Url Enclosing Structure */
struct UrlCarrier{
	std::string url;
	size_t url_id;
	std::string domain;
	size_t domain_id;
	int n_levels;

	UrlCarrier(std::string _url, size_t _url_id, std::string _domain, size_t _domain_id, int _n_levels)
	 	: url(std::move(_url)), url_id(_url_id), domain(std::move(_domain)), domain_id(_domain_id), n_levels(_n_levels){
		url.shrink_to_fit();
		domain.shrink_to_fit();
	}

	~UrlCarrier(){}
};

class Spider{

	public:
		int buffer_iUrls_size;
		int buffer_oUrls_size;

		CkSpider ck_spider;
		Url *url_manager;
		Dumper *dumper;
		Logger *logger;

		size_t log_start_id;
		int page_count;

		std::vector<std::queue<UrlCarrier>> buffer_inURLs;
		int buffer_inURLs_count;

		std::queue<UrlCarrier> buffer_outURLs;

		Spider(int buffer_iUrls_size, int buffer_oUrls_size, Logger *logger, Dumper *dumper);

		~Spider();

		void close();

		void collect(const UrlCarrier &url_carrier);
};

#endif