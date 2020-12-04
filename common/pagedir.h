/* 
 * pagedir.h
 *
 * Library of methods to check if given page directory name is an
 * actual, writable page directory and to save a page to a given 
 * directory.
 * Can be used be including pagedir.h
 *
 * Author: Sabrina Jain - CS50 - February 2020
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <webpage.h>
#include <stdbool.h>
 
/******************* save_page *****************************/
/*
 * Will save given page with the file name given if it is 
 * writable
 */
void save_page(webpage_t* page, char* fname);

/******************* is_directory *****************************/
/*
 * Will check if given directory name is a directory that can
 * be written to
 */
bool is_directory(const char* pagedir);

/********************* is_crawler_directory*************************/
/* Will check to see if the given directory name is a crawler produced
 * directory (that is, if it has a .crawler file within it). 
 *
 */
bool is_crawler_directory(char *dir);

/**********************page_load******************************/
/* Opens file with named document id in given pageDirectory, 
 * fills in the depth and URL variables from the first two 
 * lines of file. Fills in HTML with the rest of the file.
 * Returns pointer to webpage struct. 
 */
webpage_t *page_load (char *page_filename);

/******************* save_to_directory *****************************/
/*
 * Will save a page to a directory in a file named the given id 
 */
void save_to_directory(webpage_t* page, const char* pagedir, int id);



#endif // __PAGEDIR_H