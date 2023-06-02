#!/bin/bash

# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

db_name=$1

p1=-100
p2=30
p3=-95
p4=35

./build/bin/test_exhaustive $db_name $p1 $p2 $p3 $p4 > tmp

ret=$?
if [ $ret != 0 ];
then
    exit $ret
fi

sort tmp > result_exhaustive.txt


./build/bin/test_modb $db_name $p1 $p2 $p3 $p4 > tmp

ret=$?
if [ $ret != 0 ];
then
    exit $ret
fi

sort tmp > result_modb.txt

rm tmp

if diff result_exhaustive.txt result_modb.txt
then
    echo "Same results with exhaustive search. Everything seems fine."
else
    echo "PROBLEM! We got different results from exhaustive search!"
fi
