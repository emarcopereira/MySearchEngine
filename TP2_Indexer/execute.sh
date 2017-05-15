
make


mkdir -p test/runs
./test_exec -t INDEX \
			 -h test/small_collection_list.txt \
			 -s input/br_en_sp_stopwords.txt \
			 -o test/index.txt \
			 -v test/vocabulary.txt \
			 -r test/runs \
			 -l test/INDEX.log \
			 -m 5000000

#./main