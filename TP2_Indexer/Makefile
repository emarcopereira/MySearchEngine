OP=-O2 -std=c++11 -Wall 
GUMBO_OP = `pkg-config --libs --cflags gumbo`
ICU_OP = `pkg-config --libs --cflags icu-uc icu-io`
OBJ_DIR=./obj
SRC_DIR=./source
COMMAND=g++

all: clean build

build: test_indexer #test_logger #test_run_writer #test_indexer #test_parser
	#$(COMMAND) $(OP) $(OBJ_DIR)/test_run_writer.o -o test_exec
	#$(COMMAND) $(OP) $(OBJ_DIR)/logger.o $(OBJ_DIR)/test_logger.o -o test_exec
	#$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) $(OBJ_DIR)/parser.o $(OBJ_DIR)/indexer.o $(OBJ_DIR)/test_parser.o -o test_exec
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) $(OBJ_DIR)/stop_words_table.o $(OBJ_DIR)/vocabulary.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/indexer.o $(OBJ_DIR)/test_indexer.o -o test_exec
	#$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/url.o $(OBJ_DIR)/dumper.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/spider.o $(OBJ_DIR)/monitor.o $(OBJ_DIR)/crawler.o $(OBJ_DIR)/main.o $(LIB) $(OP) -o main

main: indexer
	$(COMMAND) $(OP) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

indexer: stop_words_table vocabulary logger parser 
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) -c $(SRC_DIR)/indexer.cpp -o $(OBJ_DIR)/indexer.o

parser:
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) -c $(SRC_DIR)/parser.cpp -o $(OBJ_DIR)/parser.o

logger:
	$(COMMAND) $(OP) -c $(SRC_DIR)/logger.cpp -o $(OBJ_DIR)/logger.o

vocabulary:
	$(COMMAND) $(OP) -c $(SRC_DIR)/vocabulary.cpp -o $(OBJ_DIR)/vocabulary.o

stop_words_table:
	$(COMMAND) $(OP) -c $(SRC_DIR)/stop_words_table.cpp -o $(OBJ_DIR)/stop_words_table.o

test_indexer: indexer
	$(COMMAND) $(OP) -c test/test_indexer.cpp -o $(OBJ_DIR)/test_indexer.o

test_parser: parser
	$(COMMAND) $(OP) -c test/test_parser.cpp -o $(OBJ_DIR)/test_parser.o

test_logger: logger
	$(COMMAND) $(OP) -c test/test_logger.cpp -o $(OBJ_DIR)/test_logger.o

test_run_writer:
	$(COMMAND) $(OP) -c test/test_run_writer.cpp -o $(OBJ_DIR)/test_run_writer.o

clean:
	rm -f test_exec
	rm -f main
	rm -rf *~
	rm -rf $(OBJ_DIR)/*.o