# !/bin/bash
# Testing script for indexer.c
# Author: Sabrina Jain
# Date: February 15, 2020
#
# usage: bash -v testing.sh

#copy given crawler output into data file to test
#Note: my crawler works correctly, but just to be safe!
mkdir ../data
mkdir ../data/indexes
cp -r ~cs50/data/tse-output ../data

##TESTS

## Failing tests:

# 1 argument
./indexer

# 2 arguments
./indexer ../data/test1/d1

# 3 arguments + non-existent directory
./indexer ../data/idontexist ../data/indexes/index1 

# 3 arguments + non-crawler directory
./indexer ../data/indexes ../data/indexes/index2 

#create unwritable indexfile
touch ../data/indexes/unwriteablefile
chmod -w ../data/indexes/unwriteablefile

#3 arugments + valid directory + unwriteable file
./indexer ../data/tse-output/letters-depth-0 ../data/indexes/unwriteablefile
rm -f ../data/indexes/unwriteablefile

## Passing tests:

#running indexer on CS50 playgroud (letters) depth 0
./indexer ../data/tse-output/letters-depth-0 ../data/indexes/letters-myindex-0 

#running indexer on CS50 playgroud (letters) depth 1
./indexer ../data/tse-output/letters-depth-1 ../data/indexes/letters-myindex-1

#running indexer on CS50 playgroud (letters) depth 2
./indexer ../data/tse-output/letters-depth-2 ../data/indexes/letters-myindex-2

#running indexer on CS50 playgroud (letters) depth 3
./indexer ../data/tse-output/letters-depth-3 ../data/indexes/letters-myindex-3

#running indexer on CS50 playgroud (letters) depth 4
./indexer ../data/tse-output/letters-depth-4 ../data/indexes/letters-myindex-4

#running indexer on CS50 playgroud (letters) depth 5
./indexer ../data/tse-output/letters-depth-5 ../data/indexes/letters-myindex-5

#running indexer on CS50 playgroud (letters) depth 6
./indexer ../data/tse-output/letters-depth-6 ../data/indexes/letters-myindex-6

#running indexer on Books to Scrape depth 0
./indexer ../data/tse-output/toscrape-depth-0 ../data/indexes/toscrape-myindex-0

#running indexer on Books to Scrape depth 1
./indexer ../data/tse-output/toscrape-depth-1 ../data/indexes/toscrape-myindex-1

#running indexer on Books to Scrape depth 2
./indexer ../data/tse-output/toscrape-depth-2 ../data/indexes/toscrape-myindex-2

#running indexer on Wikipedia depth 0
./indexer ../data/tse-output/wikipedia-depth-0 ../data/indexes/wikipedia-myindex-0

#running indexer on Wikipedia depth 1
./indexer ../data/tse-output/wikipedia-depth-1 ../data/indexes/wikipedia-myindex-1

#running indexer on wikipedia depth 2
./indexer ../data/tse-output/wikipedia-depth-2 ../data/indexes/wikipedia-myindex-2

#use regression testing to compare sorted results of generated indexes to 
#indexes given in tse-output (Will print "No Differences" if same)
bash testfiles/regress.sh ../data/sortedindexcompare testfiles/sortgiven/sorting_index.sh
bash testfiles/regress.sh ../data/sortedindexcompare testfiles/sortmine/sorting_index.sh

##make a directory for the loaded indexes
mkdir ../data/loadedindexes

#running indextest on a small index file (letters, depth 0)
./indextest ../data/indexes/letters-myindex-0 ../data/loadedindexes/letters-lindex-0
#Sorting
gawk -f indexsort.awk ../data/indexes/letters-myindex-0 > ../data/indexes/letters-myindex-0.sorted
gawk -f indexsort.awk ../data/loadedindexes/letters-lindex-0 > ../data/loadedindexes/letters-lindex-0.sorted
#Compare differences
if diff ../data/indexes/letters-myindex-0.sorted ../data/loadedindexes/letters-lindex-0.sorted
then 
	echo No Differences
fi

#running indextest on a large index file (wikipedia, depth 2)
./indextest ../data/indexes/toscrape-myindex-1 ../data/loadedindexes/toscrape-lindex-1
#Sorting
gawk -f indexsort.awk ../data/indexes/toscrape-myindex-1 > ../data/indexes/toscrape-myindex-1.sorted
gawk -f indexsort.awk ../data/loadedindexes/toscrape-lindex-1 > ../data/loadedindexes/toscrape-lindex-1.sorted
#Compare differences
if diff ../data/indexes/toscrape-myindex-1.sorted ../data/loadedindexes/toscrape-lindex-1.sorted
then 
	echo No Differences
fi

exit 0

