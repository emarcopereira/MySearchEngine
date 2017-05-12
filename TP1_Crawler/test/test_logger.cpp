#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include "../source/logger.hpp"

using namespace std;

int main(){
	chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
	Logger logger(0, "test", "test.logger", 1, start_time);

	/* size_t register_threadExecution_begin();
		void register_threadExecution_end(size_t transaction_id);

		size_t register_writeOnDisk_begin(char *agent);
		void register_writeOnDisk_begin(size_t transaction_id, int quant_lines);

		size_t register_downloadPage_begin(const char *domain, const char *url);
		void register_downloadPage_end(size_t transaction_id, size_t page_size);

		void register_downloadBlocked(const char *domain, int remain_time, const char *url);

		size_t register_waitCZ_begin();
		void register_waitCZ_end(size_t transaction_id);

		size_t register_manageCZ_begin();
		void register_manageCZ_end(size_t transaction_id); */

	size_t log_id = logger.register_threadExecution_begin();
	logger.register_threadExecution_end(log_id);

	log_id = logger.register_writeOnDisk_begin("logger");
	logger.register_writeOnDisk_end(log_id, 100);

	log_id = logger.register_downloadPage_begin("<domain>", 100000, "<url>", 100000, 10);
	logger.register_downloadPage_end(log_id, true, 100000);

	log_id = logger.register_waitCZ_begin();
	logger.register_waitCZ_end(log_id);

	log_id = logger.register_manageCZ_begin();
	logger.register_manageCZ_end(log_id);

	logger.register_threadExecution_end(log_id);

	return 0;
}