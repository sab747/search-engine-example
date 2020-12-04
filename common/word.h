/* 
 * word.h - header file for word module
 *
 * The word module contains a sole function, normalize_word,
 * which serves the purpose of converting all characters in
 * a string to lowercase.
 *
 * Sabrina Jain, February 2020
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdbool.h>

/**************** global types ****************/

/**************** functions ****************/

/**********************normalize_word**********************/
/* Function takes a string pointer, loops through
 * every character in the string (using its own pointer
 * as to not lose track of the entire string) and converts
 * each character to lowercase
 */
void normalize_word(char *word);

#endif // __word_H