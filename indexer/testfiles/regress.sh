#!/bin/bash
#
# regress.sh
# Sabrina Jain, February 2020 (modified from my solution
# from lab one in order to compare sorted indexes for lab
# 5)
#
# Preforms regression testing. Allows for new versions
# to be compared to stdout, stderr and exit status of old
# versions (in this case, versions of indexes)

#Check to make sure there are not less than two arguments
if [ $# \< 2 ]
then 
	echo Usage: regress.sh dirname testfilename...
	exit 1;
fi

#Check to make sure dirname isn't a file
DIR_NAME=$1
if [ ! -d $DIR_NAME ] && [ -e $DIR_NAME ]
then 
	echo ERROR: First argument \(\'$1\'\) not a directory
	exit 2;
fi

#shift arguments over by one to loop through testfilename(s)
shift 1

#loop through testfilename(s) to make sure they are readable & regular files
for tfile in "$@"
do
        if [ ! -r $tfile ] || [ ! -f $tfile ]
	then
		echo ERROR: Test file is not readable or is not a regular file
		exit 3
	fi
done

#make a directory with the name of the current date
CURR_DATE=$(date +"%Y%m%d.%H%M%S%N")
mkdir $CURR_DATE

#loop through testfilenames to create 4 files for each (copy,exit status,stdout,stderr)
for tfile in "$@"
do
	BASEFILE=$(basename $tfile);
	bash $tfile < /dev/null > $CURR_DATE/$BASEFILE.stdout 2> $CURR_DATE/$BASEFILE.stderr;
	echo $? > $CURR_DATE/$BASEFILE.status;
done

#if dirname (DIR_NAME) doesn't exist, copy CURR_DATE into new directory dirname
#otherwise, list differences (or echo 'No differences')
if [ ! -d $DIR_NAME ]
then
    mkdir "$DIR_NAME"
	cp -r $CURR_DATE/. $DIR_NAME;
	echo saved results in $DIR_NAME
	rm -rf $CURR_DATE
else
	echo saved results in $CURR_DATE
	echo comparing $CURR_DATE with $DIR_NAME
	if diff --brief $CURR_DATE $DIR_NAME
	then 
		echo No Differences
	else
		exit $?
	fi
	rm -rf $CURR_DATE
fi
 
exit 0
