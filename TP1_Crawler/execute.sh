
make

#./test_exec
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

date_time=$(date "+%F_%T")

out_folder=output/output_$date_time
pages_folder=$out_folder/pages
log_folder=$out_folder/logs
backup_folder=$out_folder/backup
stats_file=$out_folder/$date_time.stats

echo $pages_folder
echo $log_folder
echo $backup_folder
echo $stats_file

mkdir -p $pages_folder
mkdir -p $log_folder
mkdir -p $backup_folder

# ./main -s input/seeds.txt \ 	#SEEDS_FILE_PATH
# 		-p $pages_folder \ 		#PAGES_FILES_FOLDER_PATH
# 		-l $log_folder \ 		#LOGS_FILES_FOLDER_PATH
# 		-b $backup_folder \ 	#BACKUP_FOLDER_PATH
# 		-s $date_time.stats \ 	#STATS_FILE_PATH
# 		-n 80 \ 				#N_THREADS
# 		-bp 50000000 \ 			#BUFFER_PAGES_SIZE(Bytes)
# 		-bui 1000 \ 			#BUFFER_URLS_IN_SIZE(Qtd)
# 		-buo 100 \ 				#BUFFER_URLS_OUT_SIZE(Qtd)
# 		-bl 50000 \ 			#BUFFER_LOG_SIZE(Bytes)
# 		-m complex \ 			#EXECUTION_MODE
valgrind ./main -s input/seeds.txt \
		-p $pages_folder \
		-l $log_folder \
		-b $backup_folder \
		-s $stats_file \
		-n 80 \
		-bp 5000 \
		-bui 50 \
		-buo 1 \
		-bl 500 \
		-m complex 