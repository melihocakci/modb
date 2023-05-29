#!/bin/bash

# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

db_name=$1

p1=30
p2=30
p3=50
p4=50

./build/bin/test_exhaustive $db_name $p1 $p2 $p3 $p4 | sort > result_exhaustive.txt

./build/bin/test_modb $db_name $p1 $p2 $p3 $p4 | sort > result_modb.txt

if diff result_exhaustive.txt result_modb.txt
then
    echo "Same results with exhaustive search. Everything seems fine."
else
    echo "PROBLEM! We got different results from exhaustive search!"
fi
