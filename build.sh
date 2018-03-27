#!/bin/bash

# build dhbwcc
rm -r build && mkdir build && cd build && cmake .. && make
