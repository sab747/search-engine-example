# Makefile for common library
#   Builds and compiles all common functions for each
#   part of the tiny search engine into a library.
#
#
# David Kotz, April 2016, July 2017, April 2019
# Updated by Xia Zhou, July 2016
# Updated by Temi Prioleau, January 2020
# Modified for the Tiny Search Engine lab by Sabrina Jain, Febrary 2020

# object files, and the target library
L = ../libcs50
LLIBS = $L/libcs50.a 
OBJS = pagedir.o index.o word.o
LIB = common.a

# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(LLIBS) $(OBJS)

# object files depend on include files
pagedir.o: pagedir.h $L/memory.h $L/webpage.h $L/file.h
index.o: index.h $L/memory.h $L/hashtable.h $L/counters.h $L/file.h
word.o: word.h

.PHONY: clean 

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o

