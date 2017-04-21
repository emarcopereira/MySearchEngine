
make

./test_exec
#valgrind ./test_exec

#date_time=$(date "+%F_%T")
#./test_exec -s input/seeds.txt \ 	#SEEDS_FILE_PATH
#		-p output \ 		#PAGES_FILES_FOLDER_PATH
#		-l output \ 		#LOGS_FILES_FOLDER_PATH
#		-b output \ 	#BACKUP_FOLDER_PATH
#		-s stats.txt \ 	#STATS_FILE_PATH
#		-n 1 \ 				#N_THREADS
#		-bp 50000000 \ 			#BUFFER_PAGES_SIZE(Bytes)
#		-bui 1000 \ 			#BUFFER_URLS_IN_SIZE(Qtd)
#		-buo 100 \ 				#BUFFER_URLS_OUT_SIZE(Qtd)
#		-bl 50000 \ 			#BUFFER_LOG_SIZE(Bytes)
#		-m complex \ 			#EXECUTION_MODE

#date_time=$(date "+%F_%T")

#pages_folder=output/pages_$date_time
#log_folder=output/logs_$date_time
#backup_folder=output/backup_$date_time

#echo $pages_folder
#echo $log_folder
#echo $backup_folder

#mkdir -p $pages_folder
#mkdir -p $log_folder
#mkdir -p $backup_folder

#./main -s input/seeds.txt \ 	#SEEDS_FILE_PATH
#		-p $pages_folder \ 		#PAGES_FILES_FOLDER_PATH
#		-l $log_folder \ 		#LOGS_FILES_FOLDER_PATH
#		-b $backup_folder \ 	#BACKUP_FOLDER_PATH
#		-s $date_time.stats \ 	#STATS_FILE_PATH
#		-n 80 \ 				#N_THREADS
#		-bp 50000000 \ 			#BUFFER_PAGES_SIZE(Bytes)
#		-bui 1000 \ 			#BUFFER_URLS_IN_SIZE(Qtd)
#		-buo 100 \ 				#BUFFER_URLS_OUT_SIZE(Qtd)
#		-bl 50000 \ 			#BUFFER_LOG_SIZE(Bytes)
#		-m complex \ 			#EXECUTION_MODE
