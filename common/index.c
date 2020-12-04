/*
* index.c     Sabrina Jain     February 8th, 2020
*
* This file contains an implementation of the methods defined and 
* described in index.h
*       - index_new
*       - index_save
*       - index_load
*       - index_delete
*       - delete_counters (defined locally, to allow hashtable_delete
*         to delete countersets)
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <hashtable.h>
#include <file.h>
#include "index.h"
#include <counters.h>

#define LOADFACTOR .75f

/**************** global types ****************/
typedef struct index {
    hashtable_t *idx_table;
} index_t;

/**************** functions ****************/

/**************** local functions ****************/
/* not visible outside this file */
static void delete_counterset(void *item);
void save_word (void *arg, const char *key, void *item);
void save_idcounts (void *arg, const int id, int count);
int load_stopword (int c);

/*****************index_new**********************/
index_t *index_new(int num_slots) 
{
    index_t *index = assertp(malloc(sizeof(index_t)), "Error allocating memory for index");
    if (index == NULL) { //if you couldn't allocate memory for index
        return NULL; //return NULL
    }
    index->idx_table = hashtable_new(num_slots); //otherwise create hashtable
    return index; //return your index
}

/****************index_add************************/
/* see index.h for more details
 * Checks the index, the index hashtable and the word
 * pointers to makes sure they aren't null. If so, an
 * error message is printed and false is returned. False
 * is also returned if the ID was unable to be added to
 * the counterset (either the existing counterset of the
 * word or the new one). Otherwise, the index was succesfully
 * updated and the method returns true.
*/
bool index_add (index_t *index, char *word, int ID) {
    if (index != NULL) { //if valid pointer to index
        if (word != NULL) { //if the pointer to the word is good
                counters_t *word_counts; //define new counterset
                if ((word_counts = hashtable_find(index->idx_table, word)) != NULL) { //if you can't find word in hashtable
                    if (counters_add(word_counts, ID) == 0) { //invalid integer given
                        fprintf(stderr, "Integer given to index_add is invalid");
                        return false;
                    }
                    else {
                        return true;
                    }
                }
                else {
                    word_counts = counters_new(); //create a new counterset
                    if (counters_add(word_counts, ID) != 0) { //valid integer given; added correctly
                        hashtable_insert(index->idx_table, word, word_counts); //insert this key_object pair into table
                        return true;
                    }
                    else {
                        fprintf(stderr, "Integer given to index_add is invalid");
                        free(word_counts); //free pointer
                        return false;
                    }
                }
        }
        else {
            fprintf(stderr, "Word pointer given to index_add is NULL");
            return false;
        }
    }
    else {
        fprintf(stderr, "Index pointer given to index_add is NULL");
        return false;
    }
}

/************************index_get*******************************/
void *index_get (index_t *index, const char *word) {
        counters_t *word_count; //will hold counterset
        if ((word_count = (counters_t*)hashtable_find(index->idx_table, word)) != NULL) { //make sure word is in index
               return word_count;
        }
        else {
               return NULL;
        }
 }

/******************index_save*********************/
bool index_save(index_t *index, const char *index_file_name) {
    if (index != NULL) { //make sure index is valid pointer
        FILE *fp;
        if ((fp=fopen(index_file_name, "w"))!=NULL) {
            hashtable_iterate(index->idx_table, fp, save_word); //iterate through the hashtable to print index
            fclose(fp); //close file
            return true; 
        }
        else { //file couldn't be opened to write
            return false;
        }
    }
    else {
        return false;
    }
}

/******************index_load*********************/
index_t *index_load(const char *index_file_name) 
{
    FILE *fp; //create new file pointer
    if ((fp=fopen(index_file_name, "r"))!=NULL) { //try to open the file to write
        int words = lines_in_file(fp); //count number of lines in file (one line = one word)
        index_t *index = index_new((int)(words/LOADFACTOR)); //create a new index with hashtable based of # of words
        char *word; //will store word in question
        while((word = freaduntil(fp, load_stopword)) != NULL) { //read word up to first space 
            int docid; //will store the page id
            int count; //will store the count of word's appearances in page's html
            while(fscanf(fp, "%d %d ", &docid, &count) == 2) { //get id and count from file
                for (int i = 0; i < count; i++) { //for every appearance of word, add to index
                    index_add(index, word, docid); //add word appearance to index
                }
            }
            free(word); //free word pointer
        }
        fclose(fp); //close the file
        return index; //return index
    } else {
        return NULL; //can't open file to write
    }
}

/**************** index_delete ****************/
/*
 * We trust that the index has a hashtable with
 * only countersets as objects (so we pass in 
 * counters_delete)
 */
void index_delete(index_t *index) {
    hashtable_delete(index->idx_table, delete_counterset); //delete the hashtable of countersets
    free (index); //free the struct point
}

/********************LOCAL FUNCTIONS***********************/

/****************** save_word  *******************************/
void save_word (void *arg, const char *key, void *item)
{
	FILE *fp = arg; //argument should be the file to be written to
	counters_t *counters = item; //will hold ids and counts for each word
    if (fp != NULL && counters != NULL) {
        fprintf(fp, "%s ", key); //print word
	    counters_iterate(counters, fp, save_idcounts); //print ids and counters
	    fprintf(fp, "\n"); //start a new line
    }
}

/********************** save_idcounts ****************************/
void save_idcounts (void *arg, const int id, int count)
{
	FILE *fp = arg; //argument should be a file
    if (fp != NULL) {
        fprintf(fp, " %d %d", id, count); //prints page id + count
    }
}

/********************** load_stopword *****************************/
int load_stopword (int c) {
    char delimiter = ' '; //stopping character
    if (c == delimiter) { //if you are at the stopping character
        return 1; //return non-zero integer
    }
    else { //if you aren't at the stopping character
        return 0; //return 0
    }
}

/**************** delete_counters ****************/
/* Helper function in order to allow hashtable delete
 * to delete countersets (by taking in a void variable)
 */
static void delete_counterset(void *item) {
    counters_delete((counters_t*) item);
}