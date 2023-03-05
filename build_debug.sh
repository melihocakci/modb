#!/bin/sh

mkdir debug
cmake -DCMAKE_BUILD_TYPE=Debug -B debug/ -S .
cmake --build debug/
