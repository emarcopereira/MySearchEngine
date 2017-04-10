#ifndef _dumper_hpp
#define _dumper_hpp

#include <cstring>
#include <string>
#include <cstdio>
#include <queue>
#include <iostream>
//#include "logger.hpp"

class Dumper{

	private:
		/*char *folder_path;
		char *file_name;*/
		size_t buffer_size;

		FILE *file;
		std::queue<std::string> buffer;
		char file_path[200];

		void *logger;

	public:

		Dumper(const char *folder_path, const char *file_name, const size_t &buffer_size, void *logger);

		~Dumper(){}

		/* Puts piece of data to be wroten on buffer */
		void write(const char *data);

		/* Closes file */
		void close();
};

#endif