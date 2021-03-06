#ifndef _url_hpp
#define _url_hpp

#include <cstring>
#include <string>
#include <regex>
#include <functional>
#include "../chilkat/include/CkSpider.h"

class Url{

	private:
		CkSpider *ck_spider;
		std::smatch matcher;
  		std::regex expression;

	public:

		Url(CkSpider *ck_spider){
			this->ck_spider = ck_spider;
			expression = std::regex("(.*)(.br|br.|pt-br|br-)(.*)");
		}

		~Url(){} 

		bool getCleanUrl(const char *url, CkString &outStr);

		/* Return canonized url */
		inline bool isDynamic(const char *url){
			return (strpbrk(url, "@{}") != NULL);
		}

		/* Return url domain without 'www.' */
		char* getDomain(const char *url);

		/* Returns domain's hash value */
		inline size_t getStringHash(std::string &str){
			return std::hash<std::string>{}(str);
		}

		/* Counts the number of levels of a url */
		int getNumberLevels(std::string &url);

		/* Verifies if url is from br domain */ 
		inline bool isBrDomain(const char *url){
			return std::regex_match(std::string(url), expression);
		}
};

#endif