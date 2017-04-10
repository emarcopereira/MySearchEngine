#ifndef _logger_hpp
#define _logger_hpp

#include <cstring>
#include <string>
#include <cstdio>
#include <chrono>
#include "dumper.hpp"

typedef int MsgType;
typedef int MsgState;


class Logger{

	private:
		
		int thread_id;

		Dumper *dumper;
		std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

		size_t transaction_counter;

		char message[400];

	public:

		Logger(int &thread_id, const char *folder_path, const char *file_name, const size_t &buffer_size);

		~Logger(){
			delete dumper;
		}

		void close();

		int getRelativeTime();

		/* Puts part of data to be writen on buffer */
		size_t register_threadExecution_begin();
		void register_threadExecution_end(size_t transaction_id);

		size_t register_writeOnDisk_begin(char *agent);
		void register_writeOnDisk_begin(size_t transaction_id, int quant_lines, size_t quant_bytes);

		size_t register_downloadPage_begin(const char *domain, const char *url);
		void register_downloadPage_end(size_t transaction_id, size_t page_size);

		void register_downloadBlocked(const char *domain, int remain_time, const char *url);

		size_t register_waitCZ_begin();
		void register_waitCZ_end(size_t transaction_id);

		size_t register_manageCZ_begin();
		void register_manageCZ_end(size_t transaction_id);
};

#endif