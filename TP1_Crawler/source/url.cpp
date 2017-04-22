#include "url.hpp"
#include <iostream>

using namespace std;

string Url::getDomain(string &url){
	string copy(url.c_str());
	char *clean_url = const_cast<char *>(copy.c_str());
	char *aux;

	strtok(clean_url, "/");
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

	int index = 0;
	/* Pass 'http://' */
	index = strstr(c_url, "//") - c_url + 2;
	//fprintf(stderr, "\n%c\n", c_url[index]);

	/* Counts '.' until first '/' be reached */
	for(; c_url[index] != '/' && index < size; index++)
		if(c_url[index] == '.') n_levels++;

	if(strstr(c_url, "www") != NULL)
		n_levels--;

	/* Counts '/' until the end of url */
	for(int i=index; i < size; i++)
		if(c_url[i] == '/') n_levels++;

	/* In case of the las level does not ends in '/' */
	if(c_url[size-1] != '/') n_levels++;

	return n_levels;
}