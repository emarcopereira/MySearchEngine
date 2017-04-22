#include "url.hpp"
#include <iostream>

using namespace std;

char* Url::getDomain(const char *url){
	char *clean_url = const_cast<char *>(url);
	char *aux;

	strtok(clean_url, "/");
	aux = strtok(NULL, "/");

	/*if(strstr(aux, "www") != NULL){
		_ = strtok(aux, ".");
		aux = strtok(NULL, "/");
	}*/

	return aux;
}

bool Url::getCleanUrl(const char *url, CkString &outStr){
	bool sucess = this->ck_spider->CanonicalizeUrl(url, outStr);
	return sucess && (outStr.getNumChars() > 9); 
}

int Url::getNumberLevels(string &url){
	int size = url.size();
	const char *c_url = url.c_str();
	//fprintf(stderr, "\n%s", c_url);

	int n_levels = 0;

	/* Counts '/' and '.' until the end of url */
	int i = 8;
	for(; c_url[i] != '/' && i < size; i++){
		//fprintf(stderr, "%c ", c_url[i]);
		if(c_url[i] == '.') n_levels++;
	}
	for(; i < size; i++){
		//fprintf(stderr, "%c ", c_url[i]);
		if(c_url[i] == '/') n_levels++;
	}

	if(strstr(c_url, "www") != NULL)
		n_levels--;

	/* In case of the las level does not ends in '/' */
	if(c_url[size-1] != '/') n_levels++;
	//fprintf(stderr, "\n$%c$\n", c_url[size-1]);

	return n_levels;
}