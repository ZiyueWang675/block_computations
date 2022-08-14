#!/bin/make 
 
SHELL = /bin/sh 
    
CXX=g++ 
PTHREAD := -g -pthread -Wall -std=c++11 -o pthread 
FOX := mpiCC -g -Wall -std=c++11 -o mpi_fox
LIBS    :=-lm 
    
OBJS = util/matrix.cpp util/parser.cpp

.PHONY: all clean
 
all: clean pthread omp mpi mpi_fox
    
pthread: 
	$(CXX) $(PTHREAD) pthread.cpp $(OBJS) $(LIBS)

omp: 
	$(CXX) $(PTHREAD) -o omp omp.cpp $(OBJS) $(LIBS)

mpi: 
	$(CXX) $(PTHREAD) -o mpi mpi.cpp $(OBJS) $(LIBS)

mpi_fox: 
	$(FOX) mpi_fox.cpp $(OBJS) $(LIBS)

clean:
	rm -f core *.o pthread omp mpi mpi_fox
