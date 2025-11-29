#!/bin/env bash
set -ex

# use 'env' to display current environment variables
# use the following to use an IP address for the J-Link
# export PICO_SEGGER_IP=192.168.1.128

# build test binary
cmake --preset Test
cmake --build --preset app-test

# fill-in memory address of `program_arg` variable
#PROGRAM_ARG_ADDR="0x$(nm build/test/TSM_PicoW_Sensor.elf | grep program_arg | awk '{ print $1 }')"
#find src/tests/ -name '*.JLinkScript' -exec sed --regexp-extended -i "s/0x([0-9]+)/${PROGRAM_ARG_ADDR}/g" {} \;

# run all tests
ctest -v --test-dir build/Test --timeout 120 --output-on-failure
