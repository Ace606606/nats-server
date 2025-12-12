#!/bin/bash
set -e
rm -rf build-debug
mkdir -p build-debug && cd build-debug
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j$(nproc)