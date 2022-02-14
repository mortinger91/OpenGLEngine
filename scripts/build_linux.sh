#!/bin/bash

mkdir build
cd build
mkdir debug
cd debug
cmake --DCMAKE_BUILD_TYPE=DEBUG ../..
cd ..
mkdir release
cd release
cmake --DCMAKE_BUILD_TYPE=RELEASE ../..