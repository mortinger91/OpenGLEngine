#!/bin/bash

mkdir build
cd build
mkdir debug
mkdir release
cp ../imgui.ini debug/
cp ../imgui.ini release/
cd debug
cmake -DCMAKE_BUILD_TYPE=DEBUG ../..
cd ..
cd release
cmake -DCMAKE_BUILD_TYPE=RELEASE ../..
