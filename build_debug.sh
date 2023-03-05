#!/bin/sh

rm -r debug
mkdir debug
cmake -DCMAKE_BUILD_TYPE=Debug -B debug/ -S .
cmake --build debug/
