# !/bin/bash
# Testing script for crawler.c
# Author: Sabrina Jain
# Date: February 8, 2020
#
# usage: bash -v testing.sh

# Define variables
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
seedURL2=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
seedURLWiki=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
externalURL=www.google.com
nonexistentURL=http://old-www.cs.dartmouth.edu/badurlhaha.html

mkdir ../data

#####################################
### FAILING TESTS ###

# 1 argument
./crawler

# 2 arguments
./crawler $seedURL

# 3 arguments
./crawler $seedURL ../data 

# 4 arguments + externalURL
./crawler $externalURL ../data 2

#Testing 4 arguments and a nonexistent page within a internal server
#(Note: Program will still insert seed URL originally into seen URLS)"
#^ That's what it should do so we're chillin'"
./crawler $nonexistentURL ../data 2

# 4 arguments + bad page directory
./crawler $nonexistentURL data 2

# 4 arguments + invalid max depth
./crawler $nonexistentURL ../data x2

# 4 arguments + max depth less than zero
./crawler $nonexistentURL ../data -2


######################################
### PASSING TESTS ####

mkdir ../data/test1
mkdir ../data/test1/d0
mkdir ../data/test1/d1
mkdir ../data/test1/d2
mkdir ../data/test1/d3
mkdir ../data/test1/d4
mkdir ../data/test1/d5


#Testing simple site at depth 0
./crawler $seedURL ../data/test1/d0 0

#Testing simple site at depth 1
./crawler $seedURL ../data/test1/d1 1

#Testing simple site at depth 2
./crawler $seedURL ../data/test1/d2 2

#Testing simple site at depth 3
./crawler $seedURL ../data/test1/d3 3

#Testing simple site at depth 4
./crawler $seedURL ../data/test1/d4 4

#Testing simple site at depth 5
./crawler $seedURL ../data/test1/d5 5


#Repeat with different seed on same site

mkdir ../data/test2
mkdir ../data/test2/d0
mkdir ../data/test2/d1
mkdir ../data/test2/d2
mkdir ../data/test2/d3
mkdir ../data/test2/d4
mkdir ../data/test2/d5

#Testing simple site from different seed page at depth 0
./crawler $seedURL2 ../data/test2/d0 0

#Testing simple site from different seed page at depth 1
./crawler $seedURL2 ../data/test2/d1 1

#Testing simple site from different seed page at depth 2
./crawler $seedURL2 ../data/test2/d2 2

#Testing simple site from different seed page at depth 3
./crawler $seedURL2 ../data/test2/d3 3

#Testing simple site from different seed page at depth 4
./crawler $seedURL2 ../data/test2/d4 4

#Testing simple site from different seed page at depth 5
./crawler $seedURL2 ../data/test2/d5 5

echo ""

##Wikipedia

mkdir ../data/wikipedia
mkdir ../data/wikipedia/d0
mkdir ../data/wikipedia/d1

#Testing wiki playground at depth 0: 
./crawler $seedURLWiki ../data/wikipedia/d0 0

#Testing wiki playground at depth 1: 
./crawler $seedURLWiki ../data/wikipedia/d1 1

#Testing depth 2 of the wiki playground would result in over 2000 lines of code
#I don't want to put either one of us through that pain again


#testing at a greater depth

#at depth 10

mkdir ../data/test1/d10

#Testing at depth 10
./crawler $seedURL ../data/test1/d10 10

exit






