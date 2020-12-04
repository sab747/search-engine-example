# !/bin/bash
# Helper testing script for indexer.c 
# 
# Canonically sorts indexes given in
# tse output.
# 
# Stdout, stderr, and exit status
# will be compared to sorted versions
# of indexes written by my own index.
#
# Author: Sabrina Jain
# Date: February 15, 2020
# usage: bash sorting_index.sh

#Sorting letters index (depth 0)
gawk -f indexsort.awk ../data/tse-output/letters-index-0

#Sorting letters index (depth 1)
gawk -f indexsort.awk ../data/tse-output/letters-index-1

#Sorting letters index (depth 2)
gawk -f indexsort.awk ../data/tse-output/letters-index-2

#Sorting letters index (depth 3)
gawk -f indexsort.awk ../data/tse-output/letters-index-3

#Sorting letters index (depth 4)
gawk -f indexsort.awk ../data/tse-output/letters-index-4

#Sorting letters index (depth 5)
gawk -f indexsort.awk ../data/tse-output/letters-index-5

#Sorting letters index (depth 6)
gawk -f indexsort.awk ../data/tse-output/letters-index-6

#Sorting book scrape index (depth 0)
gawk -f indexsort.awk ../data/tse-output/toscrape-index-0

#Sorting book scrape index (depth 1)
gawk -f indexsort.awk ../data/tse-output/toscrape-index-1

#Sorting book scrape index (depth 2)
gawk -f indexsort.awk ../data/tse-output/toscrape-index-2

#Sorting wikipedia index (depth 0)
gawk -f indexsort.awk ../data/tse-output/wikipedia-index-0

#Sorting wikipedia index (depth 1)
gawk -f indexsort.awk ../data/tse-output/wikipedia-index-1

#Sorting wikipedia index (depth 2)
gawk -f indexsort.awk ../data/tse-output/wikipedia-index-2

exit 0