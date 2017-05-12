OP=-O2 -std=c++11 -Wall -lpthread -lresolv
GUMBO_OP = `pkg-config --libs --cflags gumbo`
ICU_OP = `pkg-config --libs --cflags icu-uc icu-io`
OBJ_DIR=./obj
SRC_DIR=./source
COMMAND=g++

all: clean build

build: test_parser
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) $(OBJ_DIR)/parser.o $(OBJ_DIR)/indexer.o $(OBJ_DIR)/test_parser.o -o test_exec
	#$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/url.o $(OBJ_DIR)/dumper.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/spider.o $(OBJ_DIR)/monitor.o $(OBJ_DIR)/crawler.o $(OBJ_DIR)/main.o $(LIB) $(OP) -o main

main: parser
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

indexer:
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) -c $(SRC_DIR)/indexer.cpp -o $(OBJ_DIR)/indexer.o

parser:
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) -c $(SRC_DIR)/parser.cpp -o $(OBJ_DIR)/parser.o

test_parser: parser indexer
	$(COMMAND) $(OP) $(GUMBO_OP) $(ICU_OP) -c test/test_parser.cpp -o $(OBJ_DIR)/test_parser.o

clean:
	rm -f test_exec
	rm -f main
	rm -rf *~
	rm -rf $(OBJ_DIR)/*.o