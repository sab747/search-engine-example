# CS50 Lab 6
## Sabrina Jain, CS50 Winter 2020

### Implementation Spec for Querier

## Summary

To implement the TSE querier, I wrote many functions in order to perform the querier's functionality within `querier.c`. These functions deal with reading in queries, parsing queries, validating queries, scoring documents based on queries, sorting documents based on score and printing documents in order. The querier uses the functionality of the common library, including the *index* (to which I added index_get()), *word* and *pagedir* modules. The querier also uses the functionality of the libcs50 library. In this implementation spec, I will go over over each module (either struct and/or set of functions) I created to implement the querier.

## Read Queries

### query struct

```
/************************query***********************/
typedef struct query { //holds query + length
    char **query_words; //words of query
    int length; //length of query
} query_t;
```

####query functions:  
```
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
```
```
/*****************query_print*********************/
/* Prints query
*/
void query_print (query_t *query);
```
```
/*****************query_delete********************/
/* Deletes query struct (frees pointers)
 */
void query_delete(query_t *query);
```
### Functions (Read Query Module)
```
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
```
```
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
```

## Validate Queries

### Functions (Validate Queries module)

```
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
```
#### Helper Functions
```
/******************* print_and_errors ***********************/
/******************* print_or_errors ***********************/
/* Helper functions just to clean up val_query a bit. Prints
 * detailed error messages depending on why query was rejected.
 * The only case not covered is if "and" or "or" is at the end
 * (handled in val_query())
 */
void print_and_errors (int i, bool and);
void print_or_errors (int i, bool and);
```
## Satisfy Query

##Functions (Satisfy Queries module)

```
/*******************************satisfy_query******************************************/
 /* Recursively goes through the query to build a counterset with documents and their 
 * scores for the query. Finds intersection (if in andsequence) with the preceding
 * counters or union (if at or) with the rest of the query. Based on Backus-Naur form.
 * 
 * Input: 
 *      query_t* query (query)
 *      int i (index)
 *      index_t *index (index for directory)
 *      counters_t *counters (documents with scores)
 * Returns:
 *      counters_t *counters (scored documents according to query)
 */
counters_t *satisfy_query (query_t *query, int i, index_t *index, counters_t *counters);
```
#### Helper functions (counterset union/intersect)
```
/**********************counters_union************************************/
/******************counters_union_helper********************************/
/* Creates a two_counters item which contains two countersets and makes the
 * first counterset the union of the two countersets using counters_iterate
 * and helper function (counters_union_helper). Used to score documents based
 * on queries.
 */
static void counters_union(counters_t *counters_a, counters_t *counters_b);
static void counters_union_helper(void *arg, const int id, const int count);
```
```
/
**********************counters_intersect************************************/
/******************counters_intersect_helper********************************/
/* Creates a two_counters item which contains two countersets and makes the
 * first counterset the intersection of the two countersets using counters_iterate
 * and helper function (counters_union_helper). Sets all counts for documents which
 * don't show up in the second set but are in the first set to 0.  Used to score
 * documents based on queries.
 */
static void counters_intersect(counters_t *counters_a, counters_t *counters_b);
static void counters_intersect_helper(void *arg, const int id, const int count);
```

### two_counters struct

```
/************************two_counters***********************/
typedef struct two_counters { //holds two countersets
    counters_t *counters_a; //first counterset
    counters_t *counters_b; //second counterset
} two_counters_t;
```

#### struct functions

```
/*********************** two_counters_new ***************************/
/* Initializes struct that can hold two counter sets
 */
two_counters_t *two_counters_new(counters_t *c_a, counters_t *c_b);
```

## Sort Docs

##Functions (Sort Docs module)

```
/*********************************sort_docs********************************************/
/* Iterates through counterset twice in order to build an array of documents with scores
 * that are ranked in descending order based on score.
 */
arr_wlen_t *sort_docs (counters_t *score_set);
```
```
/******************************count_docs*********************************************/
/* Helper function for sort_docs and counters_iterate, counts the number of non-zero
 * nodes
 * Input:
 *      counters_t *score_set (docs to sort)
 * Return:
        arr_wlen_t *sorted_docs (array of documents in decreasing order by score)
 */
void count_docs (void *arg, const int id, const int count);
```
```
/******************************insert_docs*********************************************/
/* Helper function for sort_docs and counters_iterate, inserts doc id's with their score
 * into array of such items and then swaps items until array is sorted into descending
 * order by score. A modified version of insertion sort.
 */
void insert_docs (void *arg, const int id, const int count);
```

### struct doc_score

```
/************************doc_score***********************/
typedef struct doc_score { //holds id + score
    int id; //doc id
    int score; //score
} doc_score_t;
```

#### struct functions

```
/******************************new_doc_score*******************************************/
/* Initializes a new struct that contains a document's id and its score
 */
doc_score_t *new_doc_score (int id, int count);
```

### struct arr_len

```
/************************arr_wlen***********************/
typedef struct arr_wlen {
    doc_score_t **scored_docs; //array of doc id's w/score
    int length; //length of array
} arr_wlen_t;
```
#### struct functions
```
/******************************new_arr_wlen*******************************************/
/* Initializes struct that counts array of doc id's with scores and length of array
 * Input: 
 *      doc_score_t** scored_docs (array of docs with scores)
 *      int length (length of document)
 * Returns:
 *      arr_wlen_t *new_arr_wlen
 */
arr_wlen_t *new_arr_wlen (doc_score_t** scored_docs, int length);
```
```
/******************************delete_arr_wlen*******************************************/
/* Deletes struct of type arr_wlen by freeing all doc_score_t items within it
 */
void delete_arr_wlen (arr_wlen_t *arr_wlen);
```
 
## Print Docs

### Functions (Print Docs module)

```
/*************************** print_topten*******************************************/
/* Uses a for-loop to print out the score, id and url for the top ten documents (or 
 * less if less than ten documents) documents)
 * Input:
 *      arr_wlen_t *arr_wlen (array of documents in sorted order by score + length)
 *      char *page_directory (crawler directory)
 * Return:
 *      Nothing (Prints top ten documents with scores and url)
 */
```
```
void print_topten (arr_wlen_t *arr_wlen, char *page_directory);
/*************************** print_topten_helper*****************************************/
/* Helper function for print_topten. Opens the page file, gets the url, and prints out
 * score, id and url for document. 
 */
void print_topten_helper (char *page_directory, int id, int count);
```


## Error Checking
As the functions in this querier.c are used exclusively in the file, there is not extensive error checking (would slow down recursion). *querier.c* checks the command-line arguments to make sure that they are valid. *querier.c* also parses the queries and checks that there are no non-alphabetical, non-space characters. *querier.c* then validates "and" / "or" syntax of query so that there are not errors during scoring. There are still many check points to make sure memory allocation was successful and pointers aren't null.

## Helper modules

* hashtable.h (set.h, jhash.h)
* bag.h
* counters.h
* memory.h
* webpage.h
* file.h

## Error Handling
The main method in `querier.c` handles most errors dealing with incorrect command-line arguments (assuming directories with .crawler start from page id 1 and have no gaps, as well as assuming index files are in the proper format). The main also notifies the user if there are errors in the arguments. The querier also tells user if queries are invalid (And the reason why) and moves on to next query. The testing script, `testing.sh` prints out any differences between generated indexes and given tse-output indexes and/or loaded indexes (which there should be none). 
