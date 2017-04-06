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

all: build

build: test_dumper #test_url
	#g++ $(OP) $(IOP) $(OBJ_DIR)/url.o $(OBJ_DIR)/test_url.o $(LIB) -o test_exec
	g++ $(OP) $(IOP) $(OBJ_DIR)/dumper.o $(OBJ_DIR)/test_dumper.o $(LIB) -o test_exec

dumper: 
	g++ $(OP) $(IOP) -c $(SRC_DIR)/dumper.cpp -o $(OBJ_DIR)/dumper.o

url: 
	g++ $(OP) $(IOP) -c $(SRC_DIR)/url.cpp -o $(OBJ_DIR)/url.o

test_dumper: dumper
	g++ $(OP) $(IOP) -c test/test_dumper.cpp -o $(OBJ_DIR)/test_dumper.o

test_url: url
	g++ $(OP) $(IOP) -c test/test_url.cpp -o $(OBJ_DIR)/test_url.o

#build: main crawler scheduler utils dumper binaryheap
#	g++ $(IOP) $(OBJ_DIR)/main.o $(OBJ_DIR)/.o $(LIB) -o main $(OP)

#main: crawler
#	g++ $(IOP) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o $(OP)

#crawler: scheduler utils dumper
#	g++ $(IOP) -c $(SRC_DIR)/crawler.cpp -o $(OBJ_DIR)/crawler.o $(OP)

#scheduler: utils binaryheap
#	g++ $(IOP) -c $(SRC_DIR)/scheduler.cpp -o $(OBJ_DIR)/scheduler.o $(OP)

#utils:
#	g++ $(IOP) -c $(SRC_DIR)/utils.cpp -o $(OBJ_DIR)/utils.o $(OP)

#dumper:
#	g++ $(IOP) -c $(SRC_DIR)/dumper.cpp -o $(OBJ_DIR)/dumper.o $(OP)

#binaryheap:
#	g++ -c $(SRC_DIR)/binaryheap.cpp -o $(OBJ_DIR)/binaryheap.o $(OP)

clean:
	rm -f main
	rm -rf *~
	rm -rf $(OBJ_DIR)/*.o