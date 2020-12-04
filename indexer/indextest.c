/*
* indextest.c     Sabrina Jain     February 16th, 2020
*
* This file tests my implementation of indexer for the tiny
* search engine lab by loading an index created by the indexer
* into an index data structure and then by saving it into a
* new file to make sure save/load functions are working properly
* (regression testing and sorting can be used to make sure outputs
* are equivalent)
*/

#include <stdio.h>
#include <stdlib.h>
#include <index.h>

int main (const int argc, const char *argv[]) {
    if (argc != 3) { //make sure there are exactly 3 arguments
		fprintf(stderr, "usage: ./indextest old_index_file_name new_index_file_name\n");
		return 1;
	}

    index_t *index = index_load(argv[1]); //load index from file
    index_save(index, argv[2]); //save the loaded index
    index_delete(index); //delete index

    return 0;

}