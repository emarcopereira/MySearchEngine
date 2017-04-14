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

		/* Return canonized url */
		std::string getCleanUrl(const char *url);

		/* Return url domain without 'www.' [canonize first] */
		std::string getDomain(std::string &url);

		/* Returns domain's hash value */
		inline size_t getHashDomain(std::string &domain){
			return std::hash<std::string>{}(domain);
		}

		/* Counts the number of levels of a url [canonize first] */
		int getNumberLevels(const std::string &url);

		/* Verifies if url is from br domain */ 
		inline bool isBrDomain(const char *url){
			return std::regex_match(std::string(url), expression);
		}

};

#endif