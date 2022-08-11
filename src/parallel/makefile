#!/bin/make 
 
SHELL = /bin/sh 
    
CXX=g++ 
PTHREAD := -g -lpthread -Wall -std=c++11  
LIBS    :=-lm 
    
OBJS = util/matrix.cpp util/parser.cpp

.PHONY: all clean
 
all: clean pthread omp mpi mpi_fox
    
pthread: 
	$(CXX) $(PTHREAD)   -o pthread pthread.cpp $(OBJS) $(LIBS)

omp: 
	$(CXX) $(PTHREAD)   -o omp omp.cpp $(OBJS) $(LIBS)

mpi: 
	$(CXX) $(PTHREAD)   -o mpi mpi.cpp $(OBJS) $(LIBS)

mpi_fox: 
	$(CXX) $(PTHREAD)   -o mpi_fox mpi_fox.cpp $(OBJS) $(LIBS)

clean:
	rm -f core *.o pthread omp mpi mpi_fox