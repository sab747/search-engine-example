/*
* word.c     Sabrina Jain     February 16th, 2020
*
* This file contains an implementation of the methods defined and 
* described in word.h
*       - normalize_word
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "word.h"

/*********************FUNCTIONS**********************/

/******************normalize_word*********************/
void normalize_word(char *word) 
{
    char *temp; //temporary pointer as to not lose track of string
    for (temp = word; *temp != '\0'; temp++) { //loop through every char
        *temp = tolower(*temp); //convert the character to lower case
    }
}