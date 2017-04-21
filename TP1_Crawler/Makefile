OP=-L./chilkat/lib -O2 -std=c++17 -Wall -lpthread -lresolv
IOP=-I/chilkat/include
LIB=./chilkat/lib/libchilkat-9.5.0.so
OBJ_DIR=./obj
SRC_DIR=./source
COMMAND=g++-6

UNAME_S := $(shell uname -s) 
ifeq ($(UNAME_S),Linux )
LIB=./chilkat/lib/libchilkat-9.5.0.so 
endif
ifeq ($(UNAME_S),Darwin )
LIB=./chilkat/lib/mac_libchilkat.a
endif

all: clean build

build: test_crawler #test_spider #test_monitor #test_logger #test_dumper #test_url
	#$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/url.o $(OBJ_DIR)/test_url.o $(LIB) -o test_exec
	#$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/dumper.o $(OBJ_DIR)/test_dumper.o $(LIB) -o test_exec
	#$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/monitor.o $(OBJ_DIR)/test_monitor.o $(LIB) -o test_exec
	#$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/dumper.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/test_logger.o $(LIB) -o test_exec
	#$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/url.o $(OBJ_DIR)/dumper.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/spider.o $(OBJ_DIR)/test_spider.o $(LIB) -o test_exec
	$(COMMAND) $(OP) $(IOP) $(OBJ_DIR)/url.o $(OBJ_DIR)/dumper.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/spider.o $(OBJ_DIR)/monitor.o $(OBJ_DIR)/crawler.o $(OBJ_DIR)/test_crawler.o $(LIB) $(OP) -o test_exec

main: crawler
	$(COMMAND) $(OP) $(IOP) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

crawler: spider monitor
	$(COMMAND) $(OP) $(IOP) -c $(SRC_DIR)/crawler.cpp -o $(OBJ_DIR)/crawler.o

spider: url dumper logger
	$(COMMAND) $(OP) $(IOP) -c $(SRC_DIR)/spider.cpp -o $(OBJ_DIR)/spider.o

monitor: 
	$(COMMAND) $(OP) $(IOP) -c $(SRC_DIR)/monitor.cpp -o $(OBJ_DIR)/monitor.o

logger: dumper
	$(COMMAND) $(OP) $(IOP) -c $(SRC_DIR)/logger.cpp -o $(OBJ_DIR)/logger.o

dumper: 
	$(COMMAND) $(OP) $(IOP) -c $(SRC_DIR)/dumper.cpp -o $(OBJ_DIR)/dumper.o

url: 
	$(COMMAND) $(OP) $(IOP) -c $(SRC_DIR)/url.cpp -o $(OBJ_DIR)/url.o

test_crawler: crawler
	$(COMMAND) $(OP) $(IOP) -c test/test_crawler.cpp -o $(OBJ_DIR)/test_crawler.o

test_spider: spider
	$(COMMAND) $(OP) $(IOP) -c test/test_spider.cpp -o $(OBJ_DIR)/test_spider.o

test_monitor: monitor
	$(COMMAND) $(OP) $(IOP) -c test/test_monitor.cpp -o $(OBJ_DIR)/test_monitor.o

test_logger: logger
	$(COMMAND) $(OP) $(IOP) -c test/test_logger.cpp -o $(OBJ_DIR)/test_logger.o

test_dumper: dumper
	$(COMMAND) $(OP) $(IOP) -c test/test_dumper.cpp -o $(OBJ_DIR)/test_dumper.o

test_url: url
	$(COMMAND) $(OP) $(IOP) -c test/test_url.cpp -o $(OBJ_DIR)/test_url.o

clean:
	rm -f test_exec
	rm -f main
	rm -rf *~
	rm -rf $(OBJ_DIR)/*.o