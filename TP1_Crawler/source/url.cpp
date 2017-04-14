#include "url.hpp"
#include <iostream>

using namespace std;

string Url::getCleanUrl(const char *url){
	return string(this->ck_spider->canonicalizeUrl(url));
}

string Url::getDomain(string &url){
	string copy(url.c_str());
	char *clean_url = const_cast<char *>(copy.c_str());
	char *aux, *_;

	_ = strtok(clean_url, "/");
	aux = strtok(NULL, "/");

	/*if(strstr(aux, "www") != NULL){
		_ = strtok(aux, ".");
		aux = strtok(NULL, "/");
	}*/

	return string(aux);
}



int Url::getNumberLevels(const string &url){
	int n_levels = 0;
	int size = url.size();
	const char *c_url = url.c_str();

	/* Counts '.' until first '/' be reached */
	int index = 0;
	for(; c_url[index] != '/' && index < size; index++)
		if(c_url[index] == '.') n_levels++;

	if(strstr(c_url, "www") != NULL)
		n_levels--;

	/* Counts '/' until the end of url */
	for(int i=index; i < size; i++)
		if(c_url[i] == '/') n_levels++;

	return n_levels;
}