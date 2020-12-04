/**
* set.c
*
* see set.h for more information
*
* Sabrina Jain, CS50 Lab3, February 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode { //will hold key+item
    char *k; //holds key which is a string
    void *i; //holds item of node
    struct setnode *next; //pointer to next node in the list
} setnode_t;

typedef struct set { //linked list of setnodes
  struct setnode *head; //will point to first node in the list
} set_t;

static setnode_t *setnode_new(char *key, void *item);

/**************** set_new ****************/
set_t* set_new(void)
{
    set_t *newset = count_malloc(sizeof(set_t)); //allocate memory for set
    if (newset == NULL) { //error allocating set
        return NULL;
    }
    else {
        newset->head = NULL; //make head null
        return newset; //return new set
    }
}

/**************** set_insert ****************/
bool set_insert(set_t *set, const char *key, void *item)
{
    if (set != NULL && key != NULL && item != NULL) { //check if pointers are valid
        for (setnode_t *node = set->head; node != NULL; node = node->next) { //loop through list
            if ((strcmp(key, node->k))==0) { //if its already in the set, return false
                return false;
            }
        }
        //if you exited the for loop, didn't find node with key
        setnode_t *new = setnode_new((char*)key, item); //create new key
        if (new != NULL) {
            new->next = set->head; //set new key as head of the list
            set->head = new; 
            return true; 
        }
    }
    return false; //one or more arguments false
}

/**************** setnode_new ****************/
static setnode_t *setnode_new(char *key, void *item)
{
  setnode_t *node = count_malloc(sizeof(setnode_t)); //allocate memopry for new setnode

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->k = count_malloc(sizeof(char)*strlen(key) + 1); //create space for string key
    strcpy(node->k, key); //copy key to node's copy of the key
    node->i = item; //set node's item to item
    node->next = NULL; //set next node to NULL (for now)
    return node; //return the new node
  }
}

/**************** set_find ****************/
void *set_find(set_t *set, const char *key) {
    if (set != NULL && key != NULL) { //check if pointers valid
        for (setnode_t *node = set->head; node != NULL; node = node->next) { //loop through nodes
            if ((strcmp(key, node->k))==0) { //if you find a node with a matching key
                return node->i; //return its item
            }
        }
    }
    return NULL; //node not found
}

/**************** set_print ****************/
void set_print(set_t *set, FILE *fp, 
               void (*itemprint)(FILE *fp, const char *key, void *item) ) {
    if (fp != NULL) { //if file exists
        if (set != NULL) { //if set isn't null
            fputc('{', fp);
            for (setnode_t *node = set->head; node != NULL; node = node->next) { //loop through every node
                // print this node
    
                if (itemprint != NULL) {  // print the node's item 
                    (*itemprint)(fp, node->k, node->i); 
                }
                else fputs("(null)", fp);
                if (node->next != NULL) fputc(',', fp);
            }
            fputs("}\n", fp);
            } else {
            fputs("(null)\n", fp); //if set is null, print (null)
        }
  }
}

/**************** set_iterate ****************/
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) )
{
    if (set != NULL && itemfunc != NULL) { //if pointers valid
        for (setnode_t *node = set->head; node != NULL; node = node->next) { //loop through every node
            (*itemfunc)(arg, node->k, node->i); //preform function on each item
        }
    }
}

/***************** set_delete ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) )
{
    if (set != NULL) { //if set isn't already null
        for (setnode_t *node = set->head; node != NULL; ) { //loop through nodes
            if (itemdelete != NULL) {		    // if possible...
              (*itemdelete)(node->i);	    // delete node's item
            }
            count_free(node->k); //free the key string
            setnode_t *next = node->next; //remember next node
            count_free(node);	    // free the node
            node = next;	// and move on to next
        }
        count_free(set); //free the set
    }
#ifdef MEMTEST
  count_report(stdout, "End of set_delete");
#endif
}



