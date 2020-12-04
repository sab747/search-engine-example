/* 
 * index.h - header file for CS50 index module
 *
 * An *index* is an inverted index which stores words, the
 * ID of documents each word appears in and the number of times
 * the word appears in each document.  It utilizes the hashtable
 * datastructure, which stores sets of key:object pairs (in this
 * case, word:counterset pairs).
 *
 * Sabrina Jain, February 2019
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>

/**************** global types ****************/
typedef struct index index_t;

/**************** functions ****************/

/*****************index_new**********************/
/* Caller provides:
 *   nothing
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   only inserting word: countersets pairs into the index
 *   later calling index_delete.
 */
index_t *index_new();

/****************index_add************************/
/* Provides a safeguarded way to add a proper word,
 * counterset pair into the index. Please use this 
 * method to build the index.
 * 
 * Caller provides:
 *      1. Valid pointer to a word
 *      2. Valid integer (docID) (doesn't have to
        currently exist in the counterset)
 * Will return:
 *      True if added, otherwise will print error
 *      message  and return false if word pointer is 
 *      invalid, integer is invalid, or index is NULL.
 * We trust:
 *      The user will provide a legitimate word (see indexer
 *      specs)
 */
 bool index_add (index_t *index, char *word, int ID);

 /********************index_get*********************** *****/
void *index_get (index_t *index, const char *word);
        
/******************index_save*********************/
/* Takes an index data structure and converts
 * it into file format. The format is as such:
 *       1. Each word gets its own line
 *       2. For each word, each ID of the page
 *          docID it appears in is listed alongside
 *          the number of times it appeared in
 *          the page.
 *       For example:
 *       word: docID1 count1 docID2 count2 //etc.
 * 
 * If the index is null, if the file couldn't be written to
 * or if the index is empty, returns false.
 * Otherwise, returns true.
 */
bool index_save(index_t *index, const char *index_file_name);

/******************index_load*********************/
/* Takes in an index in file format and loads it 
 * the index data structure. The format of the file
 * should be as such (same as index_save()):
 *       1. Each word gets its own line
 *       2. For each word, each ID of the page
 *          docID it appears in is listed alongside
 *          the number of times it appeared in
 *          the page.
 *       For example:
 *       word: docID1 count1 docID2 count2 //etc.
 * 
 * If the index file cannot be opened or if the file
 * is written in the wrong format, a null index will be 
 * returned and an error message will be printed to the
 * stderr.
 */
index_t *index_load(const char *index_file_name);

/**************** index_delete ****************/
/* Delete index, calling a delete function on the hashtable.
 *
 * Caller provides:
 *   valid hashtable to index with only key:object pairs that are
 *   of the form word:counterset
 * We do:
 *   if index==NULL, do nothing.
 *   if the index is in the wrong format, print error message
 *   to stderr and call hashtable delete will a NULL item_delete().
 *   
 * Notes:
 *   We pass on counters_delete as the item_delete() in hashtable_
 *   delete() if the index is of the correct format. Otherwise we 
 *   pass a null function as the item_delete() (which risks memory
 *   links, so we trust the user to use the index correctly).
 */
void index_delete(index_t *index);

#endif // __index_H
