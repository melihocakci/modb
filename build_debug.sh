#!/bin/bash

# build project
mkdir debug
cmake -DCMAKE_BUILD_TYPE=Debug -B debug
cmake --build debug
