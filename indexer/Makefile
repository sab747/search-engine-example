# Makefile for Indexer
#
# Sabrina Jain, February 2020

C = ../common
L = ../libcs50

PROGS = indexer indextest
LIBS = 
LLIBS = $C/common.a $L/libcs50.a 

# Names of macros (to use, take #s out of FLAGS)
# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
FLAGS = # -DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C 
CC = gcc
MAKE = make

.PHONY: all test clean

all: $(PROGS) 

indexer.o: $C/pagedir.h $C/index.h $L/memory.h $C/word.h
indextest.o: $C/index.h

indexer: indexer.o  
	$(CC) $(CFLAGS) indexer.o $(LLIBS) -o indexer
indextest: indextest.o  
	$(CC) $(CFLAGS) indextest.o $(LLIBS) -o indextest

#####test#####
test: indexer testing.sh
	bash -v testing.sh 
	
####clean######
clean:
	rm -f core
	rm -rf *~ *o *.dSYM
	rm -rf $(TEST_DATA)
	rm -f $(PROGS)