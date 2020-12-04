/*
* pagedir.c     Sabrina Jain     February 8th, 2020
*
* This file contains an implementation of the methods defined in pagedir.h
*       - save_page(saves page with the given file name)
*       - is_directory() (makes sure given directory name exists and is writable)
*       - save_to_directory() (saves page to directory under file name page id)
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <webpage.h>
#include <file.h>
#include "pagedir.h"


/******************* save_page *****************************/
void save_page(webpage_t* page, char* fname)
{
        FILE *fp = fopen(fname, "w");
        assertp(fp, "cannot open file for writing\n");
        fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
        fclose(fp);
}

/******************* is_directory *****************************/
bool is_directory (const char* pagedir) {
    char* backslash = "/"; 
    char* ending = ".crawler"; //fake file to test if is writable
    char* pd = malloc(strlen(pagedir)+ strlen(backslash) + strlen(ending)+ 1); //allocate memory for whole file name
    strcpy(pd, pagedir); //concatenate all strings together
    pd = strcat(pd, backslash);
    pd = strcat(pd, ending);
    FILE *fp = fopen(pd, "w"); //try to open file
    if (fp == NULL) { //if it can't be opened
        free(pd); //free pointer to directory with file string
        return false;
    }
    else { //if it can be opened, free the file
        free(pd); //free pointer to directory with file string
        fclose(fp);
        return true;
    }
}

/********************* is_crawler_directory*************************/
bool is_crawler_directory(char *dir) {
    char* ending = "/.crawler"; //fake file to test if is writable
    char* dir_crawler_file = malloc(strlen(dir)+ strlen(ending)+ 1); //allocate memory for whole file name
    strcpy(dir_crawler_file, dir); //add directory name is file name
    dir_crawler_file = strcat(dir_crawler_file, ending); //add .crawler to file name
    FILE *fp = fopen(dir_crawler_file, "r"); //try to open file
    free(dir_crawler_file);
    if (fp != NULL) { //if readable file was found
        fclose(fp); //free pointer
        return true; //return true
    }
    else { //if the file couldn't be opened to be read
        return false; //return false
    }
}

/********************page_load*********************/
webpage_t *page_load (char *page_filename) {
    FILE *fp;
    if ((fp = fopen(page_filename, "r")) != NULL) { //if we can open the file
        char *url = freadlinep(fp); //get the url (1st line)
        char *depthstring = freadlinep(fp); //get the depth (2nd line)
        int depth; //will store depth in int form
        sscanf(depthstring, "%d", &depth); //get int of depth from 2nd line
        free (depthstring); //free the pointer to the 2nd line contents
        char *html = freadfilep(fp); //read the rest of the file (html) into a string
        webpage_t *page = webpage_new(url, depth, html); //create a new webpage with extracted info
        fclose(fp); //close the file
        return page; //return this webpage (NULL is any pointers are null or error allocating memory)
    }
    else {
        return NULL; //if you can't open the file, return NULL
    }
}

/******************* save_to_directory *****************************/
void save_to_directory(webpage_t* page, const char* pagedir, int pageID) {
    char* backslash = "/";
    char ending[sizeof(pageID) + 1]; //will hold the page id
    char* fname = malloc(strlen(pagedir)+ strlen(backslash) + sizeof(pageID) + 1);
    strcpy(fname, pagedir); //start constructing file name
    sprintf(ending, "%d", pageID); //store page id in string called ending
    fname = strcat(fname, backslash); //construct file name
    fname = strcat(fname, ending);  
    save_page(page, fname); //save this file
    free(fname); //free the file name pointer
    
}


