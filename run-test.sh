#!/bin/bash

set -e

g++ -O3 ./test/test.cpp --std=c++11 -o exec-test -w

./exec-test