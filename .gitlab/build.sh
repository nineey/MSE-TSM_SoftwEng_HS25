#!/bin/env bash
set -ex

# build binary distribution
cmake --preset Debug
cmake --build --preset app-debug
cmake --preset Release
cmake --build --preset app-release
cmake --preset Test
cmake --build --preset app-test

# generate documentation
cd doxy/
doxygen
