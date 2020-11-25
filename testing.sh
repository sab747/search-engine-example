# !/bin/bash
# Testing script for querier.c
# Author: Sabrina Jain
# Date: February 24, 2020
#
# usage: bash -v testing.sh

#copy given output into data file to test
#Note: my crawler and indexer work correctly, but just to be safe 
mkdir ../data
mkdir ../data/indexes
cp -r ~cs50/data/tse-output ../data

##TESTS

## Failing tests (Command-Line Arguments):

# 1 argument
./querier

# 2 arguments
./querier ../data/tse-output/letters-depth-0

# 3 arguments + non-existent directory
./querier ../data/tse-output/idontexist ../data/tse-output/letters-index-0 

# 3 arguments + non-crawler directory
./querier ../data/indexes ../data/tse-output/letters-index-0 

#3 arugments + valid directory + nonexistent index file
./querier ../data/tse-output/letters-depth-0 ../data/tse-output/letters-index-90

#create unreadableindexfile
touch ../data/indexes/unwriteablefile
chmod -r ../data/indexes/unwriteablefile

#3 arugments + valid directory + unreadable file
./querier ../data/tse-output/letters-depth-0 ../data/indexes/unwriteablefile
rm -f ../data/indexes/unwriteablefile

## Failing tests (Query Input)

#Bad queries (see TESTING.md for details of specific cases)
./querier ../data/tse-output/letters-depth-0 ../data/tse-output/letters-index-0 < badinput.txt

##Letters test
./querier ../data/tse-output/letters-depth-0 ../data/tse-output/letters-index-0 < letterstest.txt
./querier ../data/tse-output/letters-depth-3 ../data/tse-output/letters-index-3 < letterstest.txt
./querier ../data/tse-output/letters-depth-6 ../data/tse-output/letters-index-6 < letterstest.txt
./fuzzquery ../data/tse-output/letters-index-6 10 0 | ./querier ../data/tse-output/letters-depth-6 ../data/tse-output/letters-index-0

##toscrape test
./fuzzquery ../data/tse-output/toscrape-index-1 10 0 | ./querier ../data/tse-output/toscrape-depth-1 ../data/tse-output/toscrape-index-1
./fuzzquery ../data/tse-output/toscrape-index-2 10 0 | ./querier ../data/tse-output/toscrape-depth-2 ../data/tse-output/toscrape-index-2

##wikipedia test
./querier ../data/tse-output/wikipedia-depth-2 ../data/tse-output/wikipedia-index-2 < wikitest.txt
./fuzzquery ../data/tse-output/wikipedia-index-2 10 0 | ./querier ../data/tse-output/wikipedia-depth-1 ../data/tse-output/wikipedia-index-1
./fuzzquery ../data/tse-output/wikipedia-index-2 10 0 | ./querier ../data/tse-output/wikipedia-depth-2 ../data/tse-output/wikipedia-index-2

exit
