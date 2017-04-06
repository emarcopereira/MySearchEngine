#ifndef _url_hpp
#define _url_hpp

#include <cstring>
#include "../chilkat/include/CkSpider.h"

class Url{

	private:
		CkSpider *spider;

	public:

		Url(){
			this->spider = new CkSpider();
		}

		~Url(){
			delete this->spider;
		} 

		const char* getCleanUrl(const char *url);

		const char* getDomain(const char *url);

		int getNumberLevels(const char *url);

		inline bool isBrDomain(const char *url){
			return (strstr(url, ".br") != NULL) || (strstr(url, "br.") != NULL);
		}

};

#endif