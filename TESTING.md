# CS50 Lab 6
## Sabrina Jain, CS50 Winter 2020

### Testing Summary

## Unit Testing

As I implemented the querier for the TSE project, I routinely paused at previously-identified stopping points in order to test what I had written at that point. These stopping points included:        

* After I had written code to read individual lines from stdin
* After I had written functions to parse the query
* After I had written functions to validate the query
* After I had written functions to build a counterset of documents to their query score (I printed progress comments in order to test this function, I also printed the resulting counterset)
* After I had written a short alogorithm to sort the documents based on score is descending order (using progress comments)
* After I had written the function to print the top ten (this concluded the full functionality of my program)

I also valgrinded as I went along to make sure I caught memory leaks in their tracks.

## Implementation Testing

To test my implementation of the TSE querier, I created a bash testing script called [testing.sh](testing.sh). This script tests for command-line arguments, query inputs and outputs for different pages with different depths

The first suite of test cases that I examine are the cases that should fail because their command-line arguments are invalid:  
1. 1 argument  
2. 2 arguments  
3. 3 arguments + non-existent directory  
4. 3 arguments + non-crawler produced directory  
5. 3 arguments + non-existent index file
6. 3 arguments + unreadable index file

The next suite of test cases I examine are cases in which the user enters invalid queries (in which case, the program will notify the user with a detailed message about why the query was rejected and keep reading until EOF). These invalid queries can be found in the [badinput.txt](badinput.txt) and include:    

* bad characters (such as punctuation and numbers, anything that is non-alphabetical and not a space)
* "and" at the beginning or end of query
* "or" at the beginning or end of query
* "and"/"or" adjacent in the query (in either order)

Next to test the functionality of querier I ran querier using crawler-directories/indexes for different depths for letters, toscrape and wikipedia pages. 

####Letters

I tested querier on letters at depths 0, 3 and 6 (in between cases I would be redundant). First I inputted a .txt file [letterstest.txt](letterstest.txt) of queries that I had used for unit testing while I was coding the querier. Some of these queries also include extra whitespace to demonstrate my querier can handle that as well. These tests are helpful in showing the querier works as it is easy to score the letters pages by hand so it is obvious whether querier works or not. Then, only for letters depth 6, I generated 10 random queries using `fuzzquery.c`to demonstrate that my querier worked outside of simply the test cases I could think of.

####Toscrape

I tested querier on toscrape at depth 1 and 2 using `fuzzquery.c` to generate 10 random queries each time (I'm not familiar with the contents of the toscrape index and I don't know exact word counts, so it was better to just randomly generate queries).

####Wikipedia

I similarly tested querier on wikipedia at depth 1 and 2 using `fuzzquery.c` to generate 10 random queries each time. I also included a hard-coded test case [wikitest.txt](wikitest.txt) which was shown as an example in the lecture notes in order to demonstrate that my querier generates the same output.

### Test run

An example test run is in `testing.out`, built with

	make test &> testing.out

In that file one can see the progress of the entire testing script ([testing.sh](testing.sh)). 

### Limitations

We were not given any other example output so I was unable to preform extensive regression testing in order to make sure that my querier's output matches what it should be.