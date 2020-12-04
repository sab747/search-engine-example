/*
* querier.c     Sabrina Jain     February 22nd, 2020
*
* This file contains an implementation of a querier for a 
* search engine. It contains the functions:
*       - main()
*       - prompt()
        - get_queries()
        - parse_query()
        - query_new()
        - print_query()
        - query_delete()
        - val_query()
        - print_and_errors()
        - print_and_errors()
        - satisfy_query()
        - counters_union()
        - counters_union_helper()
        - counters_intersect()
        - counters_intersect_helper()
        - sort_docs()
        - count_docs()
        - insert_docs()
        - new_doc_score()
        - new_arr_wlen()
        - delete_arr_wlen()
        - print_topten()
        - print_topten_helper()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <word.h>
#include <memory.h>
#include <pagedir.h>
#include <index.h>
#include <set.h>
#include <hashtable.h>
#include <counters.h>
#include <file.h>

/************************local structs*********************/

/************************query***********************/
typedef struct query { //holds query + length
    char **query_words; //words of query
    int length; //length of query
} query_t;

/************************two_counters***********************/
typedef struct two_counters { //holds two countersets
    counters_t *counters_a; //first counterset
    counters_t *counters_b; //second counterset
} two_counters_t;

/************************doc_score***********************/
typedef struct doc_score { //holds id + score
    int id; //doc id
    int score; //score
} doc_score_t;

/************************arr_wlen***********************/
typedef struct arr_wlen {
    doc_score_t **scored_docs; //array of doc id's w/score
    int length; //length of array
} arr_wlen_t;

/*************************functions*****************************/

/***********************fileno**********************************/
/* The fileno() function is provided by stdio,                                  
 * but for some reason not declared by stdio.h, so declare here.                
 */
int fileno(FILE *stream);
/***********************prompt**********************************/
/* Prompts user with "Query?" if reading input from a
 * keyboard; otherwise, does not prompt
 */
static void prompt(void);

/***************reading query functions***************/

/***********************get_queries*********************************/
/* Reads in query from stdin and runs rest of querier's functionality
 * on each query until EOF
 * Input: 
 *      char *page_directory (crawler directory)
 *      index_t *index (index for page)
 * Returns:
 *      Nothing (runs querier and calls other methods)
 */
void get_queries (char *page_directory, index_t *index);
/****************parse_query******************************/
/* Parses query by separating words into separate strings
 * and putting each into an array of words. If there is any
 * bad characters (aka not a space or alphabetical character),
 * will print an error message and return NULL.
 * Input: 
 *      char* query (inputted query from user)
 * Returns:
 *      query_t* query (parsed query)
 */
query_t* parse_query (char* query); 

/****************query struct functions*************/

/********************query_new******************/
/* Allocates memory for a new query struct and sets
 * its query_words (array of words in query) and 
 * length pointer.
 * 
 * Input: 
 *      char** query_words (words in query)
 *      int count (number of words in query)
 * Returns:
 *      query_t* query (query struct)
 */
query_t* query_new(char** query_words, int count);
/*****************query_print*********************/
/* Prints query
*/
void query_print (query_t *query);
/*****************query_delete********************/
/* Deletes query struct (frees pointers)
 */
void query_delete(query_t *query);
/*******************val_query********************************/
/* Returns true if query is valid according to syntax
 * requirements (and/or cannot begin or end query, and/or
 * cannot be adjacent to each other or to itself). Otherwise,
 * prints a descriptive message detailing why the query was
 * rejected and returns false.
 * Input: 
 *      query_t* query 
 *      int i (index)
 *      bool and (did an and come before?)
 * Returns:
 *      true (if valid)
 *      false (if not valid)
 */
bool val_query (query_t *query, int i, bool and);
/******************* print_and_errors ***********************/
/******************* print_or_errors ***********************/
/* Helper functions just to clean up val_query a bit. Prints
 * detailed error messages depending on why query was rejected.
 * The only case not covered is if "and" or "or" is at the end
 * (handled in val_query())
 */
void print_and_errors (int i, bool and);
void print_or_errors (int i, bool and);

/*******************************scoring functions**************************************/
/*******************************satisfy_query******************************************/
 /* Recursively goes through the query to build a counterset with documents and their 
 * scores for the query.
 * Input: 
 *      query_t* query (query)
 *      int i (index)
 *      index_t *index (index for directory)
 *      counters_t *counters (documents with scores)
 * Returns:
 *      counters_t *counters (scored documents according to query)
 */
counters_t *satisfy_query (query_t *query, int i, index_t *index, counters_t *counters);

/***************** counter union/intersection functions *******************/

/*********************** two_counters_new ***************************/
/* Initializes struct that can hold two counter sets
 */
two_counters_t *two_counters_new(counters_t *c_a, counters_t *c_b);
/**********************counters_union************************************/
/******************counters_union_helper********************************/
/* Creates a two_counters item which contains two countersets and makes the
 * first counterset the union of the two countersets using counters_iterate
 * and helper function (counters_union_helper)
 */
static void counters_union(counters_t *counters_a, counters_t *counters_b);
static void counters_union_helper(void *arg, const int id, const int count);
/**********************counters_intersect************************************/
/******************counters_intersect_helper********************************/
/* Creates a two_counters item which contains two countersets and makes the
 * first counterset the intersection of the two countersets using counters_iterate
 * and helper function (counters_union_helper). Sets all counts for documents which
 * don't show up in the second set but are in the first set to 0.
 */
static void counters_intersect(counters_t *counters_a, counters_t *counters_b);
static void counters_intersect_helper(void *arg, const int id, const int count);

/*********************************sorting functions************************************/
/*********************************sort_docs********************************************/
/* Iterates through counterset twice in order to build an array of documents with scores
 * that are ranked in descending order based on score.
 */
arr_wlen_t *sort_docs (counters_t *score_set);
/******************************count_docs*********************************************/
/* Helper function for sort_docs and counters_iterate, counts the number of non-zero
 * nodes
 * Input:
 *      counters_t *score_set (docs to sort)
 * Return:
        arr_wlen_t *sorted_docs (array of documents in decreasing order by score)
 */
void count_docs (void *arg, const int id, const int count);
/******************************insert_docs*********************************************/
/* Helper function for sort_docs and counters_iterate, inserts doc id's with their score
 * into array of such items and then swaps items until array is sorted into descending
 * order by score. A modified version of insertion sort.
 */
void insert_docs (void *arg, const int id, const int count);
/******************************new_doc_score*******************************************/
/* Initializes a new struct that contains a document's id and its score
 */
doc_score_t *new_doc_score (int id, int count);
/******************************new_arr_wlen*******************************************/
/* Initializes struct that counts array of doc id's with scores and length of array
 * Input: 
 *      doc_score_t** scored_docs (array of docs with scores)
 *      int length (length of document)
 * Returns:
 *      arr_wlen_t *new_arr_wlen
 */
arr_wlen_t *new_arr_wlen (doc_score_t** scored_docs, int length);
/******************************delete_arr_wlen*******************************************/
/* Deletes struct of type arr_wlen by freeing all doc_score_t items within it
 */
void delete_arr_wlen (arr_wlen_t *arr_wlen);


/*************************** printing methods ****************************************/

/*************************** print_topten*******************************************/
/* Uses a for-loop to print out the score, id and url for the top ten documents (or 
 * less if less than ten documents) documents)
 * Input:
 *      arr_wlen_t *arr_wlen (array of documents in sorted order by score + length)
 *      char *page_directory (crawler directory)
 * Return:
 *      Nothing (Prints top ten documents with scores and url)
 */
void print_topten (arr_wlen_t *arr_wlen, char *page_directory);
/*************************** print_topten_helper*****************************************/
/* Helper function for print_topten. Opens the page file, gets the url, and prints out
 * score, id and url for document. 
 */
void print_topten_helper (char *page_directory, int id, int count);



int main (const int argc, const char *argv[]) 
{
    if (argc != 3) { //make sure there are exactly 3 arguments
		fprintf(stderr, "usage: ./querier page_directory Index_file_name\n");
		return 1;
	}
    char *page_directory = assertp(malloc(strlen(argv[1]) + 1), 
        "Unable to allocate space for directory name\n");
    if (page_directory == NULL) { //unable to allocate space to hold directory name
        return 2;
    }
    strcpy(page_directory, argv[1]);
    if (!is_crawler_directory(page_directory)) { //check if the directory is crawler produced
        fprintf(stderr, "Not a crawler-produced directory or directory does not exist\n");
        return 3;
    }
    char *index_file_name; //copy url to string
    index_file_name = assertp(malloc(strlen(argv[2]) + 1), "Unable to allocate space for index file\n");
    if (index_file_name == NULL) { //unable to allocate space to hold file name
        return 4;
    }
    strcpy(index_file_name, argv[2]); //copy file name into string   
    FILE *indexfp = fopen(index_file_name, "r"); //try to open the file to write
    if (indexfp == NULL) { //check if the file was opened
        fprintf(stderr, "Index file not readable\n");
        return 5;
    }
    fclose(indexfp); //close file
    index_t *index = index_load(index_file_name); //load index
    if (index == NULL) {
        fprintf(stderr, "Error loading index\n");
        return 6;
    }
    get_queries(page_directory, index); //read queries, score and print ranking
    index_delete(index); //delete index
    free(page_directory); //free page directory name
    free(index_file_name); //free name of index file
    return 0;
    
}

/********************** prompt() ***********************/
/* Implemented by Temi Prioleau, provided for the CS50 Lab
 * 6 assignment. Prompts user with "Query?" if reading input 
 * from a keyboard; otherwise, does not prompt
 */
static void prompt(void)
{
    // print a prompt iff stdin is a tty (terminal)
    if (isatty(fileno(stdin))) { 
        printf("Query? ");
    }
}

/***********************get_queries*********************************/
/* Reads in query from stdin and runs rest of querier's functionality
 * on each query until EOF.
 */
void get_queries (char *page_directory, index_t *index) {
    char *query_input; //will hold line of user input
    prompt(); //prompt if reading from stdin
    while ((query_input = readlinep()) != NULL) { //read in a line of input
        if (strlen(query_input) > 0) {
            query_t *query = parse_query(query_input); //parse input and create new query
            if (query != NULL) { //if query spassed parsing (no bad characters/bad memory allocation)
                counters_t *c; //will hold documents with score
                query_print(query); //print query
                if (val_query(query, 0, false)) { //if the query is valid according to syntax rules
                    c = satisfy_query(query, 0, index, NULL); //score documents based on query
                    arr_wlen_t *satisfying_docs = sort_docs(c); //array of documents with scores in descending order
                    print_topten(satisfying_docs, page_directory); //print top ten
                    counters_delete(c); //delete counterset of query scores
                    delete_arr_wlen(satisfying_docs); //delete array of documents with scores
                }
            }
            query_delete(query); //free query pointer
            free(query_input); //free pointer to input
            fprintf(stdout, "-------------------------------------------------------------\n");
        }
        prompt();
    }
}

/****************parse_query******************************/
/* Parses query by separating words into separate strings
 * and putting each into an array of words. If there is any
 * bad characters (aka not a space or alphabetical character),
 * will print an error message and return NULL.
 */
query_t* parse_query (char* query) {
    char** words; // array of char* 
    // max # of pointers is the max # of words
    words = calloc(strlen(query)/2+1, sizeof(char*));
    int count = 0;
    for (char *rest = query; *rest != '\0'; ) {
        char *start;   // starting address of a word
        for (start = rest; isspace(*start) && *start != '\0'; start++) {} // find start of word
        for (rest = start; !isspace(*rest) && *rest != '\0'; rest++) { // find the end of word
            if (!isalpha(*rest)) { //if it isn't a space or alphabetical
                fprintf(stderr, "Bad character '%c'\n", *rest); //print error message with bad character
                free(words);
                return NULL; //return null
            }
        }
        if (*rest != '\0') { //if rest isn't already end of string
            *rest++ = '\0'; // insert '\0' to signal the end of a word
        }
        if (*start != '\0') { //if start isn't end of string already
            words[count++] = start; //add in the start
        }
    }
    for (int j = 0; j < count; j++) { //loop over all words
        normalize_word(words[j]); //convert word to lowercase
    }
    return query_new(words, count);
}

/********************query_new******************/
/* Allocates memory for a new query struct and sets
 * its query_words (array of words in query) and 
 * length pointer
 */
query_t *query_new(char** query_words, int count) {
    query_t *query; //new query pointer
    query = assertp(malloc(sizeof(query_t)), "Memory allocation error with query");
    if (query != NULL) { //if allocation worked
        query->query_words = query_words;
        query->length = count;
    }
    return query; //return query pointer
}

/*********************** query_print **********************************/
/* Prints query
*/
void query_print (query_t *query) {
    printf("Query: ");
    for (int j = 0; j < query->length; j++) { //print each word in query
        if (j == query->length -1) { //if at end, print \n instead of space
            printf("%s\n", query->query_words[j]);
        }
        else {
            printf("%s ", query->query_words[j]); //print individual word
        }
    }  
}

/********************** query_delete ************************/
/* Deletes query struct (frees pointers)
 */
void query_delete(query_t *query) {
    if (query != NULL) {
        free(query->query_words); //free array pointer
        free(query); //free query
    }
}

/************************val_query**************************/
/* Returns true if query is valid according to syntax
 * requirements (and/or cannot begin or end query, and/or
 * cannot be adjacent to each other or to itself). Otherwise,
 * prints a descriptive message detailing why the query was
 * rejected and returns false
 */
bool val_query (query_t *query, int i, bool and) {
    if (strcmp(query->query_words[i], "and") == 0) { //check if word in index i is "and"
        print_and_errors(i, and); //print detailed errors
        return false; 
    }
    else if (strcmp(query->query_words[i], "or") == 0) { //check if word in index i is "or"
        print_or_errors(i, and); //print detailed errors
        return false;
    }
    else {
        do { //enter do while loop
            if (strcmp(query->query_words[i], "and") == 0) { //if true, at word "and"
                if (i < query->length -1) { //if you aren't at the end of query
                    return val_query(query, i+1, true); //look at next sequence
                }
                else { //you are at the end of query
                    fprintf(stderr, "'and' can't be at the end of query\n");
                    return false;
                }
            } 
            else if (strcmp(query->query_words[i], "or") == 0) { //if true, at word "or"
                if (i < query->length - 1) { //if you aren't at the end of query
                    return val_query(query, i+1, false); //look at next sequence
                }
                else { //you are at the end of the query
                    fprintf(stderr, "'or' can't be at the end of query\n");
                    return false;
                }
            }
            i++;
        } while (i < query->length);
        return true;
    }
}

/******************* print_and_errors **************************/
/* Helper function just to clean up val_query a bit. Prints
 * detailed error messages depending on why query was rejected
 * for "and" errors. The only case not covered is if "and" is at 
 * the end (handled in val_query())
 */
void print_and_errors (int i, bool and) {
    if (i == 0) { //at start of query
        fprintf(stderr, "'and' can't begin query\n");
    }
    else if (!and) { //an or came before
        fprintf(stderr, "'or' and 'and' can't be adjacent\n");
    }
    else { //and came before
        fprintf(stderr, "'and' and 'and' can't be adjacent\n");
    }
}

/********************** print_or_errors **************************/
/* Helper function just to clean up val_query a bit. Prints
 * detailed error messages depending on why query was rejected
 * for "or" errors. The only case not covered is if "or" is at 
 * the end (handled in val_query())
 */
void print_or_errors (int i, bool and) {
    if (i == 0) { //at start of query
        fprintf(stderr, "'or' can't begin query\n");
    }
    else if (and) { //an and came before
        fprintf(stderr, "'and' and 'or' can't be adjacent\n");
    }
    else { //an or came before
        fprintf(stderr, "'or' and 'or' can't be adjacent\n");
    }
}

/*******************************satisfy_query******************************************/
 /* Recursively goes through the query to build a counterset with documents and their 
 * scores for the query.
 */
counters_t* satisfy_query (query_t *query, int i, index_t *index, counters_t *counters) {
    if (i == query->length) { //if at end of query
        return counters; //return counterset
    }
    else if ((strcmp(query->query_words[i], "or")) == 0) { //if at an or
        counters_t *new_running_product; //create new counterset
        new_running_product = satisfy_query(query, i+1, index, NULL); //recursively call on next index
        counters_union(counters, new_running_product); //take union (at an or)
        counters_delete(new_running_product); //delete second counterset (no need, already merged)
        return counters; 
    }
    else if ((strcmp(query->query_words[i], "and")) == 0) {
        return satisfy_query(query, i+1, index, counters); //skip and
    }
    if (counters == NULL) { //if you are at the start of an andsequence
        counters_t *running_product = counters_new(); //create new counterset
        counters_union(running_product,index_get(index, query->query_words[i])); //put everything in index at word in new counterset
        return satisfy_query(query, i+1, index, running_product); //recursively call on next index
    }
    else {
        counters_intersect(counters, index_get(index, query->query_words[i])); //get intersection (within andsequence)
        return satisfy_query(query, i+1, index, counters); //recursively call on next index
    }
}

/*********************** two_counters_new ***************************/
/* Initializes struct that can hold two counter sets
 */
two_counters_t *two_counters_new(counters_t *c_a, counters_t *c_b) {
    two_counters_t *two_counters; //two_counters point
    two_counters = assertp(malloc(sizeof(two_counters_t)), "Memory allocation error with two counters");
    if (two_counters != NULL) { //if memory allocation successful
        two_counters->counters_a = c_a; //counterset one
        two_counters->counters_b = c_b; //counterset two
    }
    return two_counters;
}

/**********************counters_union************************************/
/* Creates a two_counters item which contains two countersets and makes the
 * first counterset the union of the two countersets using counters_iterate
 * and helper function (counters_union_helper).
 * Based on in-class solution.
 */
static void counters_union(counters_t *counters_a, counters_t *counters_b)
{
    two_counters_t *two_counters = two_counters_new(counters_a, counters_b); //create new two_counters item
    counters_iterate(counters_b, two_counters, counters_union_helper); //iterate through second set
    free(two_counters); //free two counters struct
}

/******************counters_union_helper********************************/
/* Consider one item for insertion into the other set.
 * If the other set does not contain the item, insert it;
 * otherwise, update the other set's item with sum of item values. 
 * Based on in-class solution.
 */
static void counters_union_helper(void *arg, const int id, const int count)
{
  two_counters_t *two_counters = arg;
  // find the same document in counters_a
  int score_a = counters_get(two_counters->counters_a, id); //score in first set
  if (score_a == 0) {
    counters_set(two_counters->counters_a, id, count); // not found: insert it
  } else {
    counters_set(two_counters->counters_a, id, score_a + count); // add to the existing value
  }
}

/****************************counters_intersect*****************************/
/* Take the intersection of two countersets and store it in the first
 * counterset, the second counterset is unchanged.
 * Based on in-class solution.
 */
static void counters_intersect(counters_t *counters_a, counters_t *counters_b)
{
    two_counters_t *two_counters = two_counters_new(counters_a, counters_b); //create new two_counters struct
    counters_iterate(counters_a, two_counters, counters_intersect_helper); //iterate through first set
    free(two_counters);
}

/******************counters_intersect_helper********************************/
/* Consider one item for in the first counterset. If the other set does not 
 * contain the item, set count in first set for item to zero. Otherwise, 
 * update the other set's item with minimumof item values. Based on in-class 
 * solution.
 */
static void counters_intersect_helper(void *arg, const int id, const int count)
{
    two_counters_t *two_counters = arg; 
    int b_score = counters_get(two_counters->counters_b, id); //get score of doc in second counterset
    if (b_score == 0) { //if not in second counterset
        counters_set(two_counters->counters_a, id, 0);  // not found this key in set B, set the int to zero
    } else {
        if (b_score < count) { //if count in second counterset is less
            counters_set(two_counters->counters_a, id, b_score); //set first set's count for id to second counterset's
        }
    }
}

/*********************************sort_docs********************************************/
/* Iterates through counterset twice in order to build an array of documents with scores
 * that are ranked in descending order based on score.
 */
arr_wlen_t *sort_docs (counters_t *score_set) 
{
    int num_docs = 0;
    counters_iterate(score_set, &num_docs, count_docs);
    doc_score_t** scored_docs; // array of char* 
    // max # of pointers is the max # of doc_score structs
    scored_docs = (doc_score_t**)calloc(num_docs, sizeof(doc_score_t));
    counters_iterate(score_set, scored_docs, insert_docs);
    arr_wlen_t *scored_docs_wlen = new_arr_wlen(scored_docs, num_docs);
    return scored_docs_wlen;
}

 /******************************count_docs*********************************************/
/* Helper function for sort_docs and counters_iterate, counts the number of non-zero
 * nodes
 */
void count_docs (void *arg, const int id, const int count) 
{
  int *total = arg; //where total count is to be stored
  if (count > 0) {
      *total = *total + 1;
  }
}

/******************************insert_docs*********************************************/
/* Helper function for sort_docs and counters_iterate, inserts doc id's with their score
 * into array of such items and then swaps items until array is sorted into descending
 * order by score. A modified version of insertion sort.
 */
void insert_docs (void *arg, const int id, const int count) {
    if (count == 0) return; //if count is zero, ignore
    doc_score_t *doc_score = new_doc_score(id, count); //create new id w/score struct pointer
    doc_score_t **scored_docs = arg; //array of id w/score struct pointers
    int i = 0; //index
    while (scored_docs[i] != NULL) { //find first NULL index
        i++;
    }
    scored_docs[i] = doc_score; //insert doc id w/score
    while(i > 0) { //while at least at first index
        if(scored_docs[i]->score > scored_docs[i - 1]->score) { //if score is more than previous -> swap
            doc_score_t *temp = scored_docs[i - 1]; 
            scored_docs[i - 1] = scored_docs[i];
            scored_docs[i] = temp;
        }
        i--; //check next index
    }
}

/******************************new_doc_score*******************************************/
/* Initializes a new struct that contains a document's id and its score
 */
doc_score_t *new_doc_score (int id, int count) {
    doc_score_t *doc_score;
    doc_score = assertp(malloc(sizeof(doc_score_t)), "Memory allocation error with doc_score struct");
    if (doc_score != NULL) {
        doc_score->id = id;
        doc_score->score = count;
    }
    return doc_score;
}

/******************************new_arr_wlen*******************************************/
/* Initializes struct that counts array of doc id's with scores and length of array
 */
arr_wlen_t *new_arr_wlen (doc_score_t** scored_docs, int length) {
    arr_wlen_t *arr_wlen; //new arr_wlen pointer
    arr_wlen = assertp(malloc(sizeof(arr_wlen_t)), "Memory allocation error with arr_wlen");
    arr_wlen->scored_docs = scored_docs;
    arr_wlen->length = length;
    return arr_wlen;
}

/******************************delete_arr_wlen*******************************************/
/* Deletes struct of type arr_wlen by freeing all doc_score_t items within it
 */
void delete_arr_wlen (arr_wlen_t *arr_wlen) {
    for (int i = 0; i < arr_wlen->length; i++) { //loop through pointers
        free(arr_wlen->scored_docs[i]); //free doc_score pointers
    }
    free(arr_wlen->scored_docs); //free array
    free(arr_wlen); //free struct
}

/*************************** print_topten*******************************************/
/* Uses a for-loop to print out the score, id and url for the top ten documents (or 
 * less if less than ten documents) documents)
 */
void print_topten (arr_wlen_t *arr_wlen, char *page_directory) {
    int top_items = 10; //start at ten
    if (arr_wlen->length == 0) { //if no documents that match
        printf("No docments match\n"); //tell user
        return;
    }
    else if (arr_wlen->length < 10) { //if less than ten documents
        top_items = arr_wlen->length; //reset top_items
    }
    printf("Matches %d documents (ranked): \n", top_items);
    for (int i = 0; i < top_items; i++) { //go through each top item
        int id = arr_wlen->scored_docs[i]->id; //get id
        int score = arr_wlen->scored_docs[i]->score; //get score
        print_topten_helper(page_directory, id, score); //print score, id and url
    }
    
}

/*************************** print_topten_helper*****************************************/
/* Helper function for print_topten. Opens the page file, gets the url, and prints out
 * score, id and url for document. 
 */
void print_topten_helper (char *page_directory, int id, int count) {
    char pagefile[sizeof(id) + strlen("/") + 1]; //will hold "/id" in string form
    sprintf(pagefile, "/%d", id); //copy backslash and id into string
    char *file_path = assertp(malloc(strlen(page_directory) + strlen(pagefile) + 1), //allocate memory for full path
                    "unable to allocate space for full page path name");
    if (file_path != NULL) { //if you are able to allocate space for full path
        strcpy(file_path, page_directory);
        file_path = strcat(file_path, pagefile);
        FILE *fp = fopen(file_path, "r"); //pagefile
        if (fp != NULL) { //if you can open page
            char *url = freadlinep(fp); //read first line (url)
            printf("score %5d  doc  %5d : %s\n", count, id, url); //print score, id and url
            free(url);
            fclose(fp);
        }
        free(file_path); 
    }
}

