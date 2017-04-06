#include "url.hpp"

/* Return canonized url */
const char* Url::getCleanUrl(const char *url){
	return this->spider->canonicalizeUrl(url);
}

/* Return url domain without 'www.' [canonize first]*/
const char* Url::getDomain(const char *url){
	char *domain = const_cast<char*>(this->spider->getUrlDomain(url));
	const char *clean_domain;

	if(strstr(domain, "www") != NULL){
		clean_domain = strtok(domain, ".");
		clean_domain = strtok(NULL, "");
		return clean_domain;
	}

	return domain;
}

/* Counts the number of levels of a url [canonize first] */
int Url::getNumberLevels(const char *url){
	int n_levels = 0;
	int size = strlen(url);

	/* Counts '.' until first '/' be reached */
	int index = 0;
	for(; url[index] != '/' && index < size; index++)
		if(url[index] == '.') n_levels++;

	if(strstr(url, "www") != NULL)
		n_levels--;

	/* Counts '/' until the end of url */
	for(int i=index; i < size; i++)
		if(url[i] == '/') n_levels++;

	return n_levels;
}