# CS50 Lab 5
## Sabrina Jain, CS50 Winter 2020

## indexer

The TSE indexer is a program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file.

### Usage

./indexer pageDirectory indexFilename 

### Example Command Lines

./indexer ../data/test1/d1 index_file

### Exit Status
Exit status:  
  0 - success  
  1 - Invalid number of arguments  
  2 - Unable to allocate memory for directory name  
  3 - Directory doesn't exist  
  4 - Directory isn't crawler produced  
  5 - Unable to allocate memory for index file  
  6 - Unable to open index file to write

### Methods
```c
/***********************index_build*************************/
 * Recursively buils an index given a crawler produced
 * directory (given as a command-line argument) by creating an index, calling index_build_helper() and returning the index
 */
index_t *index_build (char *dir);

/**********************index_build_helper********************/
 * For each page (if the page file exists in the directory)
 * loads the page (url, depth, html) into a webpage struct, 
 * updates the index by calling update_index_wpage and calls
 * index_build_helper on the next page (id + 1). 
 */
void index_buildhelper (index_t *index, char *crawler_dir, 
	int id);
	
/*******************update_index_wpage***********************/
 * Loops through the words in a page's html. Checks to make
 * sure they have at least 3 characters, and if so, normalizes
 * the word and inserts it into the index.
 */
 void update_index_wpage(index_t *index, webpage_t *page, 
 	int id);
```

### Assumptions

No assumptions beyond those that are clear from the spec.

We assume that the given page directory, if crawler produced contains page files numbered as 1, 2, 3, etc. with no gaps. We also assume that the page files are formated according to specs (url as the 1st line, depth as the 2nd line and html as the rest of the file). Otherwise, the indexer will not work according to the requirements.

### Compilation

To compile, simply `make indexer.o`. (or `make all` to compile both [indexer.c](indexer.c) and [indextest.c](indextest.c) (see below for details on `indextest.c`)

To test, simply `make test`.
See [testing.sh](testing.sh) for details of test cases and [testing.out](testing.out) for an example test run. See [TESTING.md](TESTING.md) for more explanation of my testing process.

To valgrind, simply `make valgrind`.

To clean, simply `make clean`.

#indextest

`indextest.c` tests the correctness of the `index_save` and `index_load` functions of the *index* module. It loads a previously generated index file by the indexer into an index data structure and then saves this file to a new file (which can be compared to the old file through regression testing and sorting to make sure both are equivalent)

### Usage

./indextest old_index_file_name new_index_file_name 

### Example Command Lines

./indextest ../data/indexes/letters-myindex-1 ../data/loadedindexes/letters-lindex-1

### Exit Status
Exit status:  
  0 - success  
  1 - Invalid number of arguments  
  
### Methods

(Only main method)
  
### Assumptions

No assumptions beyond those that are clear from the spec.

Assumes that the content of the index file follows the following format:  
word docid1 count1 docid2 count2 docid3 count3 etc.  
As such this program, does not perform extensive error checking.



