# CS50 Lab 6
## Sabrina Jain, CS50 Winter 2020

## querier

The TSE Querier is a program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin (definition taken from the Requirements Spec)

### Usage

./querier page_directory index_file_name 

### Example Command Lines

./querier ../data/tse-output/letters-depth-0 ../data/tse-output/letters-index-0

### Exit Status
Exit status:  
  0 - success  
  1 - Invalid number of arguments  
  2 - Unable to allocate memory for directory name  
  3 - Directory doesn't exist  
  4 - Directory isn't crawler produced  
  5 - Unable to allocate memory for index file  
  6 - Unable to open index file to read

### Functions

For more detail on the specific functions please look at [IMPLEMENTATION.md](IMPLEMENTATION.md)

```c
static void prompt(void);
void get_queries (char *page_directory, index_t *index);
query_t* parse_query (char* query); 
query_t* query_new(char** query_words, int count);
void query_print (query_t *query);
void query_delete(query_t *query);
bool val_query (query_t *query, int i, bool and);
void print_and_errors (int i, bool and);
void print_or_errors (int i, bool and);
counters_t *satisfy_query (query_t *query, int i, index_t *index, counters_t *counters);
two_counters_t *two_counters_new(counters_t *c_a, counters_t *c_b);
static void counters_union(counters_t *counters_a, counters_t *counters_b);
static void counters_union_helper(void *arg, const int id, const int count);
static void counters_intersect(counters_t *counters_a, counters_t *counters_b);
static void counters_intersect_helper(void *arg, const int id, const int count);
arr_wlen_t *sort_docs (counters_t *score_set);
void count_docs (void *arg, const int id, const int count);
void insert_docs (void *arg, const int id, const int count);e
doc_score_t *new_doc_score (int id, int count);
arr_wlen_t *new_arr_wlen (doc_score_t** scored_docs, int length);
void delete_arr_wlen (arr_wlen_t *arr_wlen);
void print_topten (arr_wlen_t *arr_wlen, char *page_directory);
void print_topten_helper (char *page_directory, int id, int count);
```

### Assumptions

No assumptions beyond those that are clear from the spec, I implemented the full functionality of the querier.

We assume that the given page directory, if crawler produced contains page files numbered as 1, 2, 3, etc. with no gaps. We also assume that the page files are formated according to specs (url as the 1st line, depth as the 2nd line and html as the rest of the file). Otherwise, the indexer will not work according to the requirements. We also assume that the index file is in the correct format (word id1 count1 id2 count 2 etc.)

### Compilation

To compile, simply `make querier.o`. (or `make all` to compile both [querier.c](querier.c) and [fuzzquery.c](fuzzquery.c)

To test, simply `make test`.
See [testing.sh](testing.sh) for details of test cases and [testing.out](testing.out) for an example test run. See [TESTING.md](TESTING.md) for more explanation of my testing process.

To clean, simply `make clean`.





