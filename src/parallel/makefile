#!/bin/make 
 
SHELL = /bin/sh 
    
CXX = g++ 
MPIC = mpic++
PTHREAD := -g -pthread -Wall -std=c++11 -o pthread 
MPI := -g -Wall -std=c++11
OPENMP := -g -Wall -fopenmp 
LIBS := -lm 
    
OBJS = util/matrix.cpp util/parser.cpp

.PHONY: all clean
 
all: clean serial pthread omp mpi mpi_fox mpi_cannon
    
serial: 
	$(CXX) -g -Wall -std=c++11 -o serial serial.cpp $(OBJS) $(LIBS)

pthread: 
	$(CXX) $(PTHREAD) pthread.cpp $(OBJS) $(LIBS)

omp: 
	$(CXX) $(OPENMP) -o omp omp.cpp $(OBJS) $(LIBS)

mpi: 
	$(MPIC) $(MPI) -o mpi mpi.cpp $(OBJS) $(LIBS)

mpi_fox: 
	$(MPIC) $(MPI) -o mpi_fox mpi_fox.cpp $(OBJS) $(LIBS)

mpi_cannon: 
	$(MPIC) $(MPI) -o mpi_cannon mpi_cannon.cpp $(OBJS) $(LIBS)

clean:
	rm -f core *.o serial pthread omp mpi mpi_fox mpi_cannon
