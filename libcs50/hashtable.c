/**
*  hashtable.c
*
* see hashtable.h for more information
*
* Sabrina Jain, CS50 Lab3, February 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "jhash.h"
#include "memory.h"
#include "set.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/

typedef struct hashtable {
  struct set **hasharray; //stores a pointer to an array of sets
  int num_slots;          //keeps track of number of slots in array for quick reference
} hashtable_t;


/**************** hashtable_new ****************/
hashtable_t *hashtable_new(const int num_slots)
{
    hashtable_t *newhashtable = count_malloc(sizeof(hashtable_t)); //create pointer to a hashtable
    if (newhashtable == NULL) { //error allocating hashtable
        return NULL;
    }
    else { //if allocation successful
        newhashtable->hasharray = count_calloc(num_slots, sizeof(set_t*)); //allocate hasharray of sets
        if (newhashtable->hasharray == NULL) { //error allocating hasharray 
            return NULL;
        }
        else { //if allocation of hasharray succesful
            newhashtable->num_slots = num_slots; //simple tracker for number of slots
            return newhashtable; //return the new hashtable
        } 
    }
}

/**************** hashtable_insert ****************/
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) 
{
    //if arguments are valid
    if (ht != NULL && key != NULL && item != NULL) {
        int hashpos = JenkinsHash(key, ht->num_slots); //hash your key
        if (ht->hasharray[hashpos] == NULL) { //if there is no existing set in slot
            ht->hasharray[hashpos] = set_new(); //create new set at slot
        }
        if (ht->hasharray[hashpos] != NULL) { //if there is a set at slot
            if (set_find(ht->hasharray[hashpos], key) == NULL) { //if you can't find the key
                set_insert(ht->hasharray[hashpos], key, item); //insert the item
                return true;
            }
        }
    }
    return false; //if arguments are valid, error allocating new set or key already in table
}

/**************** hashtable_find ****************/
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht != NULL && key != NULL) {
        int hashpos = JenkinsHash(key, ht->num_slots); //hash your key
        if (ht->hasharray[hashpos] == NULL) { //if there is no existing set in slot
            return NULL; //no set exists, so item can't be in set at spot, return NULL
        }
        else { //if there is a set at slot
            void *item = set_find(ht->hasharray[hashpos], key); //try to find key and get item in set
            return item; //returns item(if the item is null, will return NULL)
        }
    }
    else return NULL; 
}

/**************** hashtable_print ****************/
void hashtable_print(hashtable_t *ht, FILE *fp, 
    void (*itemprint)(FILE *fp, const char *key, void *item)) { 

    if (fp != NULL) { //if file exists
        if (ht != NULL) { //if ht is valid pointer

            for (int i = 0; i < ht->num_slots; i++) {
                if (itemprint != NULL) {  // print valid itemprint function
                // print the set at this slot
                fprintf(fp, "%d: ", i);
                    if (ht->hasharray[i] != NULL) { //if there is a set at index
                        set_print(ht->hasharray[i], fp, itemprint);
                    }
                    else fprintf(fp, "\n"); //if no set, still need to start new line
                }
                
            }
        } else {
            fputs("(null)\n", fp);
        }
    }                    
}

void hashtable_iterate(hashtable_t *ht, void *arg,
    void (*itemfunc)(void *arg, const char *key, void *item) ) {
    if (ht != NULL && itemfunc != NULL) { //if pointers valid
        for (int i = 0; i < ht->num_slots; i++) { //loop through every index
            if (ht->hasharray[i] != NULL) { //if there is a set at index
                set_iterate(ht->hasharray[i], arg, itemfunc); //iterate through set
            }
        }
    }
}


/**************** hashtable_delete ****************/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ) {
    if (ht != NULL) { //if hashtable isn't null
        for (int i = 0; i < ht->num_slots; i++) { //loop through every index
            	    // remember what comes next
            if (itemdelete != NULL) {		    // if possible...
                if (ht->hasharray[i] != NULL) { //if there is a set at index
                    set_delete(ht->hasharray[i], itemdelete); //delete set
                 }
            }
        }
        count_free(ht->hasharray); //free pointer to array of sets
        count_free(ht); //free hashtable
    }
#ifdef MEMTEST
  count_report(stdout, "End of hashtable_delete");
#endif
}