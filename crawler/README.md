# CS50 Lab 3
## Sabrina Jain, CS50 Winter 2020

### crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”. (This definition is taken from the given design spec.)

### Usage

.crawler seedURL pageDirectory maxDepth 

### Example Command Lines

.crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ../data/test0 7

### Exit Status
Exit status:  
  0 - success  
  1 - Invalid number of arguments  
  2 - Unable to allocate space for url  
  3 - URL not internal  
  4 - Invalid page directory  
  5 - Invalid max depth (characters start argument 'max_depth')  
  6 - Max depth less than zero  
  7 - Unable to allocate memory and new a crawler struct  

### Methods
```c
/* 
 * crawler_new()
 * Creates a new crawler struct
 */
static crawler_t *crawler_new(const int max_depth, const char *pagedir, char *url);

/* 
 * crawl_start()
 * Starts crawling process by creating a new webpage for the url
 * and inserting it into the bag of webpages to be crawled.
 */
void crawl_start(crawler_t *crawler, int depth, char *url);

/* 
 * crawler()
 * Takes a webpage out of the bag, checks that its depth is below
 * the max depth and, if so, calls scanURLs
 */
bool crawl(crawler_t *crawler);

/* 
 * scanURLs()
 * Looks at all URLs on a webpage, if internal URL and not
 * in the hashtable of seen URLs, it calls crawl_start() on 
 * that URL.
 */
void scanURLs (crawler_t *crawler, webpage_t *page);

/*
 * delete_crawler()
 * Deletes the crawler
 */
void delete_crawler (crawler_t *crawler);

/*
 * Deletes pointer passed through
 */ 
void item_delete (void *item);
```

### Assumptions

No assumptions beyond those that are clear from the spec.

The seedURL should point to a valid page within an internal server (or else an error message to the standard error will be printed). The pageDirectory must already exist and must be writable (elsewise an error message to the stderr is printed). MaxDepth must be larger greater than or equal to zero (elsewise an error message is printed). The argument maxDepth can also not begin with any characters (though the number can be followed by them, the program with just ignore excess characters).

### Compilation

To compile, simply `make crawler.o`.

To test, simply `make test`.
See [testing.sh](testing.sh) for details of test cases and [testing.out](testing.out) for an example test run.

To clean, simply `make clean`.
