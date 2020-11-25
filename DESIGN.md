# CS50 Lab 6
## Sabrina Jain, CS50 Winter 2020

# Design Spec for Querier

## User interface

The querier's interface with the user is through the command-line, which will have two arguments, and through the standard input.

```
./querier page_directory index_file_name
```

## Inputs and Outputs

Input: The inputs are the command-line parameters (see User interface section) and text from standard input (each line represents an input for one query).

Output: We print the cleaned up query (Backus-Naur form) and a list of the top ten (or less) documents that satisfy that query, along with their scores and URLs. We then repeat until EOF.

Example:

```
Query: we or went or to or the or home
Matches 6 documents (ranked):   
score     3  doc      1 : http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
score     1  doc      2 : http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
score     1  doc      3 : http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
score     1  doc      4 : http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
score     1  doc      5 : http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
score     1  doc      6 : http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
```

## Functional decomposition into modules

(note, other than main, these modules each describe a set of functions and/or structs

* main
* *read query* (query)
* *validate query* 
* *satisfy query* (two counters)
* *sort docs* (document with score, array with length)
* *print docs*


## Pseudo code for logic/algorithmic flow

1. Execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules:
	* load index
3. Read in a line from standard input
4. Parse line to split into individual words regardless of white space
	* Also check that there are no "bad characters" (non-alphabetical/non-space) characters
5. Validate query according to Backus-Naur form (no "or"/"and" at beginning or end of query, no "or"/"and" adjacent to itself or one another)
6. Print out cleaned query for the user to see
7. Score each document based on the query according to Backus-Naur logic  
	* Intersect (for "and") and union (for "or") documents with their scores to find documents that satisfy and their final scores
8. Insert each document into an array of documents with their scores using an insertion sort-like method of insertion (add to end of array, swap until sorted)
	* Order is decreasing by score
9. Print out the top 10 documents (or less if less than 10 satisfy query) along with their score and url
	* If no documents match, print "No documents match"
9. Repeat step #3-9 until end of input is reached

## Dataflow through modules:
* main: Parses command line arguments and loads index
* *read query* (query): Reads in query from user (one line of input) and parses it to remove whitespace and to separate it into individual words. Also notifies user if bad characters are in in query (if so, rejects query)
* *validate query* : Makes sure query is in Backus-Naur form
* *satisfy query* : Scores each documents according to query to find out which documents satisfy query
* *sort docs* (document with score, array with length) : Puts documents with their scores into a sorted array in decreasing order (ignores documents that don't satisfy query)
* *print docs* : Prints top ten (or less) documents that satisfy query, along with their scores and urls

## Testing plan:
Throughout writing the code I will repeatedly compile all files to check for compilation errors (though I will read through my code beforehand to minimize errors found). I will pass arguments to the querier in order to make sure they are being properly taken in and/or rejected if invalid. I will also repeatedly unit-test by testing at these stopping points:    

* After I had written code to read individual lines from stdin
* After I had written functions to parse the query
* After I had written functions to validate the query
* After I had written functions to build a counterset of documents to their query score (I printed progress comments in order to test this function, I also printed the resulting counterset)
* After I had written a short alogorithm to sort the documents based on score is descending order (using progress comments)
* After I had written the function to print the top ten (this concluded the full functionality of my program)

I will also valgrind when doing these intitial tests (and throughout the process) to catch memory leaks in their tracks. Finally I will run `querier.c` through a suite of test cases (both hard-coded and random using `fuzzquery.c`) to make sure my querier is working according to the requirements specs.