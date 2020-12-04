/**
* counters.c
*
* see counters.h for more information
*
* Sabrina Jain, CS50 Lab3, February 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counternode {  //will hold each key+count
    int key;                    //key
    int count;                    //count
    struct counternode *next; //next node
} counternode_t;

typedef struct counters {
  struct counternode *head; //points to first counternode in linked list
} counters_t;

static counternode_t *counternode_new(int key);

/**************** counters_new ****************/
counters_t* counters_new(void)  
{
    counters_t *counterset = count_malloc(sizeof(counters_t)); //allocate memory for counters struct
    if (counterset == NULL) { //error allocating counterset
        return NULL;
    }
    else {                       
        counterset->head = NULL;    //set counterset head to NULL
        return counterset;          //return the new counterset
    }
}

/**************** counters_new ****************/
int counters_add(counters_t *ctrs, const int key) 
{
    if (ctrs != NULL && key >= 0) {     //make sure parameters aren't null;
        for (counternode_t *node = ctrs->head; node != NULL; node = node->next) { //loop through each element
            if (node->key == key) { //if you found a node with the key 
                node->count = node->count + 1; //increase  by one
                if (node->count > 0)  //if count is greater than or equal to zero
                {
                    return node->count; //return count
                }
                else return 0; //otherwise, return 0
            }
        }
        //exited for loop
        counternode_t *new = counternode_new(key); //create new node since doesn't currently exist
        if (new != NULL) {   //if no error in creating new node
            new->next = ctrs->head; //set the next node to the head of the current list
            ctrs->head = new; //made your new node the head of the list
            if (ctrs->head->count >0) { //if count is greater than or equal to zero
                return ctrs->head->count; //return count
            }
            else return 0; 
        }
        else {
            return 0; 
        }
    }
    else {
        return 0;
    }
}

/**************** counternode_new ****************/
counternode_t *counternode_new(int key)
{
  counternode_t *node = count_malloc(sizeof(counternode_t)); //allocate memory for new node

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->key = key;     //key
    node->count = 1;       //make count 1
    node->next = NULL; //next node is null (for now)
    return node;       //return node
  }
}

/**************** counters_get ****************/
int counters_get(counters_t *ctrs, const int key) {
    if (ctrs != NULL && key >= 0) { //make sure arguments are valid pointers
        for (counternode_t *node = ctrs->head; node != NULL; node = node->next) { //loop through every node
            if (node->key == key) { //if you find a node with the key
                return node->count; //return the node's count
            }
        }
        return 0; //exited the for loop, didn't find node, return 0
    }
    return 0; //return 0 if arguments invalid
}

/**************** counters_set ****************/
bool counters_set(counters_t *ctrs, const int key, const int count) {
    if (ctrs != NULL && key >= 0 && count >= 0) { //if arguments are valid
        for (counternode_t *node = ctrs->head; node != NULL; node = node->next) { //loop through nodes
            if (node->key == key) { //if you find a node with the key
                node->count = count; //make its count equal to the count passed into method
                return true; //return true b/c count changed
            }
        }
        //exited loop without finding node
        counternode_t *new = counternode_new(key); //make a new counternode
        if (new != NULL) {
            new->next = ctrs->head; //put node at the head of the list
            ctrs->head = new;
            ctrs->head->count = count; //set its count 
            return true; //return true b/c node created
        }
    }
    return false; //error, return false
}

/**************** counters_print ****************/
void counters_print(counters_t *ctrs, FILE *fp) {
    if (fp != NULL) { //if file exists
        if (ctrs != NULL) { //if counterset exists
            fputc('{', fp); 
            for (counternode_t *node = ctrs->head; node != NULL; node = node->next) { //loop through every node
                // print this node (key=count)
                fprintf(fp, "(%d,%d)" ,node->key, node->count);
                if (node->next != NULL) { //put a comma between each node
                    fputs(", ", fp);
                }
            }
            fputs("}\n", fp);
            } 
        else {
            fputs("(null)", fp); //if null print null
        }
    }
}

/**************** counters_iterate ****************/
void counters_iterate(counters_t *ctrs, void *arg, 
                      void (*itemfunc)(void *arg, 
                    const int key, const int count)) 
{
    if (ctrs != NULL && itemfunc != NULL) { //if counterset exists and function is valid
        for (counternode_t *node = ctrs->head; node != NULL; node = node->next) { //loop through every node
            (*itemfunc)(arg, node->key, node->count); //run function on item
        }
    }
}

/***************** counters_delete ****************/
void counters_delete(counters_t *ctrs) 
{
    if (ctrs != NULL) { //if counterset exists
        for (counternode_t *node = ctrs->head; node != NULL; ) { //loop through every node
            node->key = 0;
            node->count = 0;
            counternode_t *next = node->next;	    // remember what comes next
            count_free(node);	    // free the node
            node = next;	// and move on to next
        }
        count_free(ctrs); //free counterset
    }
#ifdef MEMTEST
  count_report(stdout, "End of counters_delete");
#endif
}



