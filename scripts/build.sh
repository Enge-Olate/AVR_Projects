#!/bin/bash

cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/avr-toolchain.cmake
cmake --build  build

