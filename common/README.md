# CS50 Lab 5
## Sabrina Jain, CS50 Winter 2020

## common

Library of methods to deal with saving pages to a directory, handling an index and normalizing words. For example, in the *pagedir* module, is_directory() can check if a given directory name is an actual, writable directory. Other methods included help save a page to a given directory in a file named the given numerical id. The *index* module have data structs and functions to build, edit, save and load an inverted index. The *word* module has one function, `normalize_word`, to convert words to lowercase.

### Assumptions

No assumptions beyond those that are clear from the spec.

The `is_crawler_directory` function in *pagedir* module assumes that if there exists a .crawler file in the directory that can be read, it is a proper, crawler-produced directory. `page_load` similarily assumes that page files are in the format outlined in the requirements spec. The index *module* relies on the index being built properly. `index_load` requires the index file to be in the format outlined in the specs. 

### Compilation

To build `common.a`, run `make`. 

To clean, simply `make clean`.

## Overview

 * `index` - the **index** data structure
 * `pagedir` - a library of functions to deal with pages in TSE
 * `word` - function to convert word to all lowercase

