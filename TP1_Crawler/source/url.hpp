#include <cstring>
#include "CkSpider.h"

class Url{

	private:
		Spider spider;

	public:

		Url(){
			this->spider = new Spider();
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

}