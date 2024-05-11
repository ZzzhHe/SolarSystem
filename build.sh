#!/bin/bash
mkdir -p build
cd build
cmake -S ../ -B .
make
cd ..
./bin/SolarSystem
