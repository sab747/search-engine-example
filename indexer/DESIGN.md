# CS50 Lab 5
## Sabrina Jain, CS50 Winter 2020

# Design Spec for Indexer

## User interface

The indexer's interface with the user is through the command-line, which will have two arguments.

```
./indexer pageDirectory indexFilename
```

## Inputs and Outputs

Input: The inputs are the command-line parameters (see User interface section)  
Output: We save an inverted index of words to the IDs of the documents they appear in + the count of times that the word appears in the document. Example of format:   
(word docID count docID2 count2)  

## Functional decomposition into modules
main
index
index_build
NormalizeWord
index_save

## Pseudo code for logic/algorithmic flow

1. Execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules 
3. Go to the first file in the crawler-produced directory and create a 
webpage struct to hold the url, depth and html listed in the file (read
in directly from the file, not fetched from the URL). 
4. Loop through every word in the HTML
5. Normalize the word using NormalizeWord
6. Update the word count for the document in the index accordingly
7. Repeat step #3-6 until there are no more files left to be examined in 
the directory.
8. Save the index to indexFilename using index_save

## Dataflow through modules:
...main: parses parameters, creates an index and calls index_build
...index: an inverted index data structure to store words, the docIDs
they occur in and the number of times they appear in each docID
...index_build: loops through every page in the directory and and updates
word counts in the index accordingly
...NormalizeWord: converts each word to lowercase and checks that its not less 
than three characters
...index_save: saves the index in a file format

## Testing plan:
Throughout writing the code I will repeatedly compile all files to check for compilation errors (though I will read through my code beforehand to minimize errors found). Next I will pass arguments to the indexer in order to make sure they are being properly taken in and/or rejected if invalid. I will also valgrind when doing these intitial tests (and throughout the process) to catch memory leaks in their tracks. I will then keep testing each method as I go by slowly building up the main function in indexer (and of course deleting functions that work and don't belong in main in the final implementation). Finally I will run `indexer.c` and `indextest.c` through a suite of test cases to make sure my indexer is working according to the requirements specs.