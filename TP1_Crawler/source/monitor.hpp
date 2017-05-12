#ifndef _monitor_hpp
#define _monitor_hpp

#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <chrono>

#include <thread>
#include <shared_mutex>
#include <mutex>
#include <condition_variable>

class Monitor{

	public://private:

		const char *stats_file_path;
		FILE *stats_file;
		std::chrono::steady_clock::time_point start_time;

		std::unordered_map<int, int> values;
		std::shared_mutex values_mutex;

		bool stop_command;

		std::thread monitor_thread;
		std::condition_variable stop_command_cv;
		std::shared_mutex stop_command_mutex;

	//public:

		Monitor(const char *stats_file_path, std::chrono::steady_clock::time_point start_time);

		~Monitor();

		void write_values();

		template <class Period>
		void monitoring(std::chrono::duration<long  int, Period> period){
			bool stop;
			std::mutex mtx;
			std::unique_lock<std::mutex> lck(mtx);

			while(this->stop_command_cv.wait_for(lck, period) == std::cv_status::timeout){
				this->write_values();

				{std::lock_guard<std::shared_mutex> lock(this->stop_command_mutex);
					stop = this->stop_command;
				}

				if(stop){
					fprintf(stderr, "[Monitor] Stop command detected!\n");
					break;
				}
			}
		}

		template <class Period>
		inline void start_monitoring(std::chrono::duration<long  int, Period> period){
			this->monitor_thread = std::thread(&Monitor::monitoring<Period>, this, period);
		}

		inline void stop_monitoring(){
			{std::shared_lock<std::shared_mutex> lock(this->stop_command_mutex);
				this->stop_command = true;
			}
			this->stop_command_cv.notify_one();
			this->monitor_thread.join();
		}

		inline void include_value(int id, int init_value){
			{std::lock_guard<std::shared_mutex> lock(this->values_mutex);
				this->values[id] = init_value;
			}
		}

		inline void update_value(int id, int value){
			{std::shared_lock<std::shared_mutex> lock(this->values_mutex);
				this->values[id]++;
			}
		}
};

#endif