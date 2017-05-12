#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include "../source/crawler.hpp"

using namespace std;

Monitor *monitor;

void stopExecution(int signum){
	monitor->stop_monitoring();
	//delete monitor;
	fprintf(stderr, "terminando\n");

	//exit(EXIT_SUCCESS);
}

int main(){
	fprintf(stderr, "foi1\n");
	chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
	monitor = new Monitor("test/test.stats", start_time);

	signal(SIGINT, stopExecution);

	monitor->include_value(0, 0);
	fprintf(stderr, "foi2\n");
	monitor->start_monitoring(chrono::seconds(10));
	fprintf(stderr, "foi3\n");
	int value = 60000;
	while(value--){
		monitor->update_value(0, value);
	}
	fprintf(stderr, "foi4\n");

   	monitor->monitor_thread.join();

	delete monitor;

	return 0;
}