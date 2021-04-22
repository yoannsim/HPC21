CXX=g++
CXXFLAGS= -g -std=c++11 -Wall -pedantic -I../../../lib/benchmark/include/
CXXLDFLAGS_G= -L../../../lib/benchmark/build/src/ -lbenchmark -lpthread

CC=gcc
CFLAGS= -O3 -fno-tree-vectorize -std=c11 -Wall -Wextra -g

BIN_G=gbench

SRC_CPP=$(wildcard *.cpp)
OBJCXX=$(SRC_CPP:%.cpp=%.o)

SRC_C=$(wildcard *.c)

OBJC=$(SRC_C:%.c=%.o)

all: $(BIN_H) $(BIN_G)

$(BIN_G): $(OBJC) gbench.o
	$(CXX) -o $(BIN_G) $^ $(WEBLDFLAGS_G) $(CXXLDFLAGS_G)

gbench.o: gbench.cpp
	$(CXX) $(WEBFLAGS_G) $(CXXFLAGS) -o $@ -c $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o
	rm $(BIN_G) $(BIN_H)

rung:	$(BIN_G)
	./$(BIN_G)

run_web: $(BIN_G)
	./$(BIN_G) --color_print=false

run:	rung


.PHONY : all
.PHONY : clean
