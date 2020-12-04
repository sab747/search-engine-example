/*
* indexer.c     Sabrina Jain     February 8th, 2020
*
* This file contains an implementation of a indexer for a 
* search engine. It contains the functions:
*       - main()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pagedir.h>
#include <memory.h>
#include <word.h>
#include <index.h>

#define SLOTNUM 400

/*******************FUNCTIONS***********************/
index_t *index_build (char *crawler_dir);
void index_buildhelper (index_t *index, char *crawler_dir, int id);
void update_index_wpage(index_t *index, webpage_t *page, int id);


int main (const int argc, const char *argv[]) {
    if (argc != 3) { //make sure there are exactly 3 arguments
		fprintf(stderr, "usage: ./indexer pageDirectory IndexFileName\n");
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
    FILE *indexfp = fopen(index_file_name, "w"); //try to open the file to write
    if (indexfp == NULL) { //check if the file was opened
        fprintf(stderr, "Couldn't open file to write\n");
        return 5;
    }
    else { 
        fclose(indexfp); //close file (will open later)
    }

    index_t *index = index_build(page_directory); //build the index
    index_save(index, index_file_name); //save the index
    index_delete(index); //delete the index

    free(page_directory); //free directory name
    free(index_file_name); //free file name

    return 0;

}

/******************index_build***********************/
/* Builds index by creating new index pointer,
 * calling the recursive function, index_build_helper
 * on first page and (once recursion completed, index 
 * has been built) returns index.
 *
 * User provides:
 *  1. crawler_dir
 *
 * Returns:
 *  1. built index
 */
index_t *index_build (char *crawler_dir) {
    index_t *index = index_new(SLOTNUM); //create new index pointer
    index_buildhelper(index, crawler_dir, 1); //call recursive index_build_helper
    return index; //return the index
}

/*********************index_buildhelper*******************************************/
/* Determine path to page file given the id and the crawler directory,
 * try to load the page (if not page doesn't exist in directory or can't
 * be opened). if you can load the page, call update_index_wpage to update
 * index for words found on the page and call index_build_helper on the next
 * page (id + 1). Utilizes recursion
 *
 * User provides:
 *  1. index
 *  2. crawler_dir
 *  3. id
 *
 * Returns:
 *  Nothing (void)
 */
void index_buildhelper (index_t *index, char *crawler_dir, int id) {
    char pagefile[sizeof(id) + strlen("/") + 1]; //will hold "/id" in string form
    sprintf(pagefile, "/%d", id); //copy backslash and id into string
    char *dir_path = assertp(malloc(strlen(crawler_dir) + strlen(pagefile) + 1), //allocate memory for full path
                    "unable to allocate space for full directory path name");
    if (dir_path != NULL) { //if you are able to allocate space for full path
        strcpy(dir_path, crawler_dir);
        dir_path = strcat(dir_path, pagefile);
        webpage_t *page; 
        if ((page = page_load(dir_path))!= NULL) {
            update_index_wpage(index, page, id); //update index for all words on the page
            index_buildhelper(index, crawler_dir, id + 1); //repeat, incrementing id by one
        }
        webpage_delete(page);
        free(dir_path); //free full path to page file
    }
    
}

/************************update_index_page*****************************/
/* For the given page, loop through all words in the HTML using
 * webpage_getNextWord, check if the word is greater than two charcters
 * in length, convert word to lowercase and add it to the index
 *
 * User provides:
 *  1. index
 *  2. page
 *  3. id
 *
 * Returns:
 *  Nothing (void)
 */
void update_index_wpage(index_t *index, webpage_t *page, int id) {
    int pos = 0; //position within the html
    char *word; //will hold the word in question
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(word) > 2) { //make sure length is three or greater
            normalize_word(word); //convert word to lower case
            index_add(index, word, id); //add the word to the index
        }
        free(word); //free word pointer
    }
}