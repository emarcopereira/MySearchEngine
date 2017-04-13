OP=-L./chilkat/lib -O2 -std=c++11 -Wall -lpthread -lresolv
IOP=-I/chilkat/include
LIB=./chilkat/lib/libchilkat-9.5.0.so
OBJ_DIR=./obj
SRC_DIR=./source

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIB=./chilkat/lib/libchilkat-9.5.0.so
endif
ifeq ($(UNAME_S),Darwin)
    LIB=./chilkat/lib/mac_libchilkat.a
endif

all: clean build

build: test_dumper #test_logger #test_dumper #test_url
	#g++ $(OP) $(IOP) $(OBJ_DIR)/url.o $(OBJ_DIR)/test_url.o $(LIB) -o test_exec
	g++ $(OP) $(IOP) $(OBJ_DIR)/dumper.o $(OBJ_DIR)/test_dumper.o $(LIB) -o test_exec
	#g++ $(OP) $(IOP) $(OBJ_DIR)/dumper.o $(OBJ_DIR)/logger.o $(OBJ_DIR)/test_logger.o $(LIB) -o test_exec

logger: dumper
	g++ $(OP) $(IOP) -c $(SRC_DIR)/logger.cpp -o $(OBJ_DIR)/logger.o

dumper: 
	g++ $(OP) $(IOP) -c $(SRC_DIR)/dumper.cpp -o $(OBJ_DIR)/dumper.o

url: 
	g++ $(OP) $(IOP) -c $(SRC_DIR)/url.cpp -o $(OBJ_DIR)/url.o

test_logger: logger
	g++ $(OP) $(IOP) -c test/test_logger.cpp -o $(OBJ_DIR)/test_logger.o

test_dumper: dumper
	g++ $(OP) $(IOP) -c test/test_dumper.cpp -o $(OBJ_DIR)/test_dumper.o

test_url: url
	g++ $(OP) $(IOP) -c test/test_url.cpp -o $(OBJ_DIR)/test_url.o

clean:
	rm -f test_exec
	rm -f main
	rm -rf *~
	rm -rf $(OBJ_DIR)/*.o