/*
* crawler.c     Sabrina Jain     February 8th, 2020
*
* This file contains an implementation of a crawler for a 
* search engines. It contains the functions:
*       - main()
*       - crawler_new() (Instanstiates crawler)
*       - crawl_start() (1 of three functions to recursively crawl through webpages)
*       - crawl() (2 of three functions to recursively crawl through webpages)
*       - scan_URLs() (3 of three functions to recursively crawl through webpages)
*       - crawler_delete() (deletes the crawler)
*       - item_delete() (deletes a valid pointer)
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"
#include "pagedir.h"

typedef struct crawler {  //will hold each key+count
    int max_depth;
    int currID;
    char *pagedir;
    bag_t *bag_pages;
    hashtable_t *seen_urls;
} crawler_t;

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

int main (const int argc, const char *argv[]) {
    if (argc != 4) { //make sure there are exactly 4 arguments
		fprintf(stderr, "usage: crawler seedURL pageDirectory maxDepth\n");
		return 1;
	}
    char *url; //copy url to string
    if ((url = assertp(malloc(strlen(argv[1]) + 1), "Unable to allocate space for url\n")) != NULL) {
        strcpy(url, argv[1]);
    }
    else {
        return 2;
    }
    if (!IsInternalURL(url)) { //make sure the URL is internal
        fprintf(stderr, "URL is not internal\n");
        return 3;
    }
    const char *page_directory = argv[2]; //get page directory
    if (!is_directory(page_directory)) {
        fprintf(stderr, "Page directory given is invalid\n");
        return 4;
    }
    int max_depth;
    if (sscanf(argv[3], "%d", &max_depth) != 1) {
      fprintf(stderr, "Invalid page depth given\n");
      return 5;
    } 
    if (max_depth < 0) {
        fprintf(stderr, "Max depth less than 0\n");
        return 6;
    }
    crawler_t *crawler = crawler_new(max_depth, page_directory, url); //create new crawler struct
    if (crawler == NULL) { //make sure that the crawler was newed properly
        return 7;
    }

    delete_crawler(crawler); //free memory associated with the crawler
    return 0;

}

/*************************** crawler_new ********************************/
static crawler_t *crawler_new(const int max_depth, const char *pagedir, char* url) {
    crawler_t *new_crawler; //new crawler pointer
    new_crawler = assertp(malloc(sizeof(crawler_t)), "Unable to allocate space for crawler\n"); //allocate memory for crawler struct
    if (max_depth >= 0) { //make sure max_depth is valid
        new_crawler->max_depth = max_depth; //set max_depth
    } 
    else return NULL;
    new_crawler->currID = 1; //set page ID to zero
    new_crawler->pagedir = assertp((malloc(sizeof(char) * strlen(pagedir) + 1)), "Could not allocate memory for page directory name\n");
    strcpy(new_crawler->pagedir, pagedir); //copy page directory name to crawler
    bag_t *bag_pages = bag_new(); //create new bag to hold pages
    if (bag_pages != NULL) { //make sure bag was created
        new_crawler->bag_pages = bag_pages;
    }
    else { //otherwise, error message
        fprintf(stderr, "Could not create bag for crawler struct\n");
        return NULL; 
    }
    hashtable_t *seen_urls = hashtable_new(10); //create new hashtable to hold visited urls
    if (seen_urls != NULL) { //make sure hashtable was created
        new_crawler->seen_urls = seen_urls;
    }
    else { //otherwise, error message
        fprintf(stderr, "Could not create hashtable for crawler struct");
        return NULL; 
    }
    void *emptystring = malloc(3); //create empty string to insert into hashtable
    strcpy(emptystring, "");
    hashtable_insert(new_crawler->seen_urls, url, emptystring); //insert seed URL
    printf("Inserted %s (seed url)\n", url); //let user know seed url has been insert
    crawl_start(new_crawler, 0, url); //start crawling
    return new_crawler; //return new crawler

}

/*************************** crawl_start ********************************/
void crawl_start (crawler_t *crawler, int depth, char *url) {
    webpage_t *page = webpage_new(url, depth, NULL); //make new webpage for starting url
    bag_insert(crawler->bag_pages, page); //insert that page into bag
    crawl(crawler); //start crawling
}

/*************************** crawl ********************************/
bool crawl(crawler_t *crawler) {
    webpage_t *curr_page = (webpage_t*)bag_extract(crawler->bag_pages); //extract a page from bag
    if (curr_page == NULL) { //make sure a non-NULL page was extracted
        return false; //otherwise, return false
    }
    else { //if a webpage was extracted
        if(webpage_fetch(curr_page)) {
            save_to_directory(curr_page, crawler->pagedir, crawler->currID); //save url, depth and HTML to directory
            crawler->currID = crawler->currID + 1; //increment page ID
            if (webpage_getDepth(curr_page) >= crawler->max_depth) { //make sure the page's depth is less than max depth
                webpage_delete(curr_page); //get rid of the current page pointer
                return false; //return false (page is below max depth)
            }
            else {
                scanURLs(crawler, curr_page); //scan the URLs of the page
                webpage_delete(curr_page); //delete the page pointer
                return true; //return true
            }
        } //get the HTML of the url
        else { //invalid page
            fprintf(stderr, "Invalid page, no HTML\n");
            webpage_delete(curr_page);
            return false;
        }
    }
}

/*************************** scan_URLs ********************************/
void scanURLs (crawler_t *crawler, webpage_t *page) {
    printf("Exploring URL: %s\n", webpage_getURL(page));
    int pos = 0; //keeps track of your position in the HTML
    char *result; //will hold found URLs
    //an empty string in order to use hashtable_insert
    while ((result = webpage_getNextURL(page, &pos)) != NULL) { 
        if (IsInternalURL(result)) { //make sure it is an internal URL
            if (!hashtable_find(crawler->seen_urls, result)) {
                void *emptystring = malloc(3);
                strcpy(emptystring, "");
                hashtable_insert(crawler->seen_urls, result, emptystring);  //try to insert it into the hashtable
                printf("Inserted %s\n", result);
                crawl_start(crawler, webpage_getDepth(page) + 1, result); //if you can, start crawling that URL
                
            }
            else {
                free(result);
            }
        }
        else {
            free(result);
        }
    }
}

/*************************** item_delete ********************************/
void item_delete (void *item) {
    if (item != NULL) { //if the pointer isn't null
        free(item); //free the pointer
    }
}

/*************************** delete_crawler ********************************/
void delete_crawler (crawler_t *crawler) {
    free(crawler->pagedir); //free the page directory of the crawler
    bag_delete(crawler->bag_pages, webpage_delete); //free the crawler's bag
    hashtable_delete(crawler->seen_urls, item_delete); //free the hashtable
    free(crawler); //free the crawler
}







