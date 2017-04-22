#ifndef _dumper_hpp
#define _dumper_hpp

#include <cstring>
#include <string>
#include <cstdio>
#include <queue>
#include <fstream>
#include "../chilkat/include/CkSpider.h"

class Dumper{

	private:
		size_t buffer_size;
		size_t buffer_count;
		char file_path[200];

		std::filebuf file_buffer;
    	std::ostream *output_stream;

		//void *logger;

	public:

		Dumper(const char *folder_path, const char *file_name, const size_t buffer_size);

		~Dumper();

		/* Puts piece of data to be wroten on buffer */
		void write(CkString &data);

		void write(std::string &data);

		/* Closes file */
		void close();
};

#endif