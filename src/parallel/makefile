#!/bin/make 
 
SHELL = /bin/sh 
    
UTIL := util 
CXX=g++ 
PTHREAD := -g -lpthread -Wall -std=c++11  
LIBS    :=-lm 
    
OBJS = $(UTIL)/matrix.cpp $(UTIL)/parser.cpp

.PHONY: all clean
 
all: clean pthread
    
pthread: pthread.o  
    $(CXX) $(PTHREAD)   -o pthread pthread.cpp $(OBJS) $(LIBS) 
    
    
clean: 
    rm -f core *.o pthread 