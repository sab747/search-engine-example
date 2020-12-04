# Makefile for Querier
#
# Sabrina Jain, February 2020

C = ../common
L = ../libcs50

PROGS = querier fuzzquery
LIBS = 
LLIBS = $C/common.a $L/libcs50.a 

# Names of macros (to use, take #s out of FLAGS)
# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
FLAGS = # -DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=gnu11 -ggdb $(FLAGS) -I$L -I$C 
CC = gcc
MAKE = make

.PHONY: all test clean

all: $(PROGS) 
 
querier.o: $L/file.h $C/word.h $L/memory.h $C/pagedir.h $C/index.h $L/set.h $L/hashtable.h $L/counters.h
fuzzquery.o: $L/file.h $L/memory.h

querier: querier.o  
	$(CC) $(CFLAGS) querier.o $(LLIBS) -o querier
fuzzquery: fuzzquery.o  
	$(CC) $(CFLAGS) fuzzquery.o $(LLIBS) -o fuzzquery

#####test#####
test: querier testing.sh
	bash -v testing.sh 
	
####clean######
clean:
	rm -f core
	rm -rf *~ *o *.dSYM
	rm -rf $(TEST_DATA)
	rm -f $(PROGS)