CC=g++
CFLAGS=-c -Wall -std=c++11

all: nsp

nsp: main.o nsp.o solver.o
	$(CC) main.o nsp.o solver.o -o nsp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

nsp.o: nsp.cpp
	$(CC) $(CFLAGS) nsp.cpp

solver.o: solver.cpp
	$(CC) $(CFLAGS) solver.cpp

clean:
	rm *o nsp