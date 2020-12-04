# CS50 Lab 4
## Sabrina Jain, CS50 Winter 2020

### Implementation Spec for Indexer

## Summary

To implement the TSE indexer, I created an index module (in the common library, has both an index struct and functions) in order to create the functionality needed to build the inverted index. `indexer.c` uses this index module to build an index (which it defines its own function to carry out) and to save an index. Both the index module and `indexer.c` use the functionality of the *pagedir* and *file* modules (found in the common library and libcs50 library respectively). `indexer.c` also uses the *word* module (found in the common library). In this implementation spec, I will go over over each module I added (with also a note of two functions i added in the *pagedir* module and detail the data structures and/or functions I created to implement the indexer.

## index

### index struct

```
typedef struct index {
    hashtable_t *idx_table;
} index_t;
```

* idx_table: holds the inverted index, using words as keys and countersets (pageIDs : count of word in page) as objects.

####Functions:  
```
index_t *index_new();
```
1. Allocates space to the index  
2. Creares a new hashtable to store the index

```
bool index_add (index_t *index, char *word, int ID)
```
1. Checks if index and word are valid pointers (other wise returns false)  
2. Creates a new counterset pointer  
3. Check if word is in hashtable already
4. If so, add the id to the counterset (increment by one if the id already is in the counterset) (return true if successful
5. If not, add the word to the hashtable with a new counterset containing the id (return true if successful)
6. If you can't add the id to the counterset (either existing or new), then returns false


```
bool index_save(index_t *index, const char *index_file_name)
```
1. Create new file pointer
2. Try to open the file to write (if you can't, return false)
3. If you can, iterate through the index's hashtable (idx_table) to print key object pairs by passing a function to print words (which contains a call to a function to print the counterset of counts)
4. Close the file
5. Return true  

Local helper functions: 
`void save_word (void *arg, const char *key, void *item);`  
`void save_idcounts (void *arg, const int id, int count);`

```
index_t *index_load(const char *index_file_name) 
```
1. Try to open file to write (otherwise return NULL)
2. If so, count the number of lines in file (each line is one word)
3. Create new index with hashtable of words/LOADFACTOR slots (I am using the standard load factor, 0.75, that programming languages such as java use)
4. Read word using a function in the *file* module to read up until the first space
5. Read in each id and its count of appearances of the word (Use a for loop to add each appearance into index)
6. Repeat for every line (representing each word
7. Free word pointer
8. Close file
9. Return Index

Local helper functions: `int load_stopword (int c)`

```
void index_delete(index_t *index)
```
1. Delete hashtable
2. Free index pointer

Local helper functions: 
`static void delete_counterset(void *item)`

##indexer

### Functions:

```
index_t *index_build (char *crawler_dir)
```   
1. Creates a new index pointer with defined macro SLOTNUM for hashtable slots
2. Call index_build_helper (a recursive function) for page 1
3. Return index (which will be once all pages have been loaded and all words have been added to the index.

```
void index_buildhelper (index_t *index, char*crawler_dir, int id)
```
1. Creates a string representing path to individual page file
2. If path can be created, create a page pointer
3. Try to load the page by calling page_load (if NULL returned, index_build_helper won't call itself and recursion will end) 
4. If you can load the page, update the index with the words on the page by calling update_index_wpage
5. Call index_build_helper on id+1
6. Delete the webpage pointer
7. Free the page file path pointer

```
void normalize_word(char *word);
```
1. Loop through all the words of the webpage using webapge_getNextWord
2. Inside the loop: 1) check if the word is three or more characters long 2) normalize the word (convert it to lowercase) 3) add it to the index with the page id
3. Free word pointer

## word

```
void normalize_word(char *word);
```
1. create a temporary char* pointer
2. Loop through all of the characters in word
3. For each character, call the ctype.h function, tolower(int c) on each character

## pagedir

```
bool is_crawler_directory(char *dir);
```
1. Tries to open .crawler file in directory by creating a path through strcpy and strcat
2. If it can open the .crawler file to read, returns true
3. If it can't open the file to read, returns false

```
webpage_t *page_load (char *page_filename)
```
1. Try to open given page file to read (if not, return NULL)
2. Read the first line into a string pointer called url
3. Read the second line in a string pointer called depthstring
4. Read the first integer in second line (assumes correct format)
5. Read the rest of the file into a string pointer called html
6. Create webpage using url, depth and html
7. Close file
8. Return the webpage

## Error Checking
Throughout my implementation of indexer, I repeatedly make sure that the pointers passed to each function are valid (that is, not null). `indextest.c` also tests the validity of functions in the *index* module by calling index_load on an existing index file and index_save on the created index (which combined also call index_add and index_new). `indextest.c` also calls index_delete. The `testing.sh` script provided also checks created indexes against given tse-output index files.

## Helper modules

* hashtable.h (set.h, jhash.h)
* bag.h
* counters.h
* memory.h
* webpage.h
* file.h

## Error Handling
The main method in `indexer.c` handles most errors dealing with incorrect arguments (assuming directories with .crawler start from page id 1 and have no gaps). The main also notifies the user if there are errors in the arguments. The testing script, `testing.sh` prints out any differences between generated indexes and given tse-output indexes and/or loaded indexes (which there should be none).

