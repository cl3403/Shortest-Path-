OPTS = -g -Wall -std=c++11

sptest: sptest.cc sptest.h shortestPath.o BinaryHeap.o
	g++ ${OPTS} -pthread -o sptest sptest.cc shortestPath.o BinaryHeap.o

shortestPath.o: shortestPath.cc shortestPath.h BinaryHeap.o
	g++ ${OPTS} -pthread -c shortestPath.cc 

BinaryHeap.o: BinaryHeap.cc BinaryHeap.h
	g++ ${OPTS} -c BinaryHeap.cc

currency: currency.cc shortestPath.o 
	g++ ${OPTS} -o currency currency.cc shortestPath.o BinaryHeap.o

suggestwords: suggestwords.cc
	g++ ${OPTS} -o suggestwords suggestwords.cc

mapqueue: mapqueue.cc
	g++ ${OPTS} -pthread -o mapqueue mapqueue.cc

all: sptest currency suggestwords mapqueue
