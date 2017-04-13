#ifndef _url_hpp
#define _url_hpp

#include <cstring>
#include "../chilkat/include/CkSpider.h"

class Url{

	private:
		CkSpider *ck_spider;

	public:

		Url(CkSpider *ck_spider){
			this->ck_spider = ck_spider;
		}

		~Url(){} 

		/* Return canonized url */
		const char* getCleanUrl(const char *url);

		/* Return url domain without 'www.' [canonize first] */
		const char* getDomain(const char *url);

		/* Counts the number of levels of a url [canonize first] */
		int getNumberLevels(const char *url);

		/* Verifies if url is from br domain */ 
		inline bool isBrDomain(const char *url){
			return (strstr(url, ".br") != NULL) || (strstr(url, "br.") != NULL);
		}

};

#endif