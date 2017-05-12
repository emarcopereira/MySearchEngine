#include "spider.hpp"
#include "project_definitions.hpp"
#include <iostream>

using namespace std;

Spider::Spider(int buffer_iUrls_size, int buffer_oUrls_size, Logger *logger, Dumper *dumper){
	this->logger = logger;
    this->log_start_id = logger->register_threadExecution_begin();

    this->buffer_iUrls_size = buffer_iUrls_size;
	this->buffer_oUrls_size = buffer_oUrls_size;

	this->url_manager = new Url(&(this->ck_spider));
	this->dumper = dumper;

    this->page_count = 0;

    for(int i=0; i<MAX_URL_LEVEL; i++)
        this->buffer_inURLs.push_back(queue<UrlCarrier>());
    this->buffer_inURLs.shrink_to_fit();
    this->buffer_inURLs_count = 0;
}

Spider::~Spider(){
    //this->close();

    if(this->url_manager != NULL)
	   delete this->url_manager;
}

void Spider::close(){
    dumper->write(" ||| ");
    /*dumper->close();*/

    logger->register_threadExecution_end(this->log_start_id);

	/* Dumping outUrls Buffer back to the queue */

    /* Dumping inUrls Buffer to the queue */
}

void Spider::collect(const UrlCarrier &url_carrier){
    /* Trying to download page */
	ck_spider.Initialize(url_carrier.domain.c_str());
    ck_spider.AddUnspidered(url_carrier.url.c_str());

    size_t log_id = logger->register_downloadPage_begin(url_carrier.domain.c_str(), url_carrier.domain_id, url_carrier.url.c_str(), url_carrier.url_id, url_carrier.n_levels);
    bool success = ck_spider.CrawlNext();
	
    /* Skipping collecion in case of unsucessful download */
    if(not success){
        logger->register_downloadPage_end(log_id, success, 0);
        return;
    }

    CkString collected_html;
    ck_spider.get_LastHtml(collected_html);
    logger->register_downloadPage_end(log_id, success, collected_html.getNumChars());

    /* Saving collected page */
    CkString collected_url, buffer;
    ck_spider.get_LastUrl(collected_url);
    dumper->write("||| \n");
    dumper->write(collected_url.getString());
    dumper->write("\n | \n");
    dumper->write(collected_html.getString());
    page_count++;

    int n_levels;
    const char *c_url;
    size_t domain_id, url_id;
    CkString aux_url;

    /* Adding InBound Links */
    int size = ck_spider.get_NumUnspidered();
    for(int i = 0; i < size; i++){
    	ck_spider.GetUnspideredUrl(0, collected_url);
      	ck_spider.SkipUnspidered(0);

        c_url = collected_url.getString();
        //fprintf(stderr, "%s\n", c_url);

        /* Removing urls out of br domain */
        if(not url_manager->isBrDomain(c_url))
            continue;

        /* Removing dynamic urls */
        if(url_manager->isDynamic(c_url))
            continue;

        /* Removing in case of failed canonization*/
        if(!url_manager->getCleanUrl(c_url, aux_url))
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
        //fprintf(stderr, "Added InUrl: %s Domain: %s NLevels: %d\n", canonized_url.c_str(), domain.c_str(), n_levels);

      	buffer_inURLs[n_levels-1].push(UrlCarrier(canonized_url, url_id, domain, domain_id, n_levels));
        buffer_inURLs_count++;
    }
    
    /* Adding OutBount Links */
    size = ck_spider.get_NumOutboundLinks();
    int num_out = 0;
    for(int i = 0; i < size; i++){
		if(not ck_spider.GetOutboundLink(i, collected_url)) continue;

        c_url = collected_url.getString();
        //fprintf(stderr, "%s\n", c_url);

        /* Removing urls out of br domain */
        if(not url_manager->isBrDomain(c_url))
            continue;

        /* Removing dynamic urls */
        if(url_manager->isDynamic(c_url))
            continue;

        /* Removing in case of failed canonization*/
        if(!url_manager->getCleanUrl(c_url, aux_url))
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
        //fprintf(stderr, "Added OutUrl: %s Domain: %s NLevels: %d\n", canonized_url.c_str(), domain.c_str(), n_levels);

        buffer_inURLs[n_levels-1].push(UrlCarrier(canonized_url, url_id, domain, domain_id, n_levels));
        buffer_inURLs_count++;
        num_out++;
    }
    logger->register_linkExtraction(buffer_inURLs_count-num_out, num_out);

    ck_spider.ClearOutboundLinks();
    ck_spider.ClearSpideredUrls();
    ck_spider.ClearFailedUrls();
}