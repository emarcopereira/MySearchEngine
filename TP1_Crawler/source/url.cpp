#include "url.hpp"

const char* Url::getCleanUrl(const char *url){
	return this->ck_spider->canonicalizeUrl(url);
}

const char* Url::getDomain(const char *url){
	char *clean_url = const_cast<char*>(url);
	char *aux, *_;

	_ = strtok(clean_url, "/");
	aux = strtok(NULL, "/");

	if(strstr(aux, "www") != NULL){
		_ = strtok(aux, ".");
		aux = strtok(NULL, "/");
	}

	return aux;
}

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