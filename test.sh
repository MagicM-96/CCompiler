#!/bin/bash
# build dhbwcc
rm -r build && mkdir build && cd build && cmake .. && make
cd ../testing/tests/
./test.sh

# tests#
# ./dhbwcc ../testing/tests/success/t01.c
# echo "Test 01"
# sleep 1
# ./dhbwcc ../testing/tests/success/t02.c
# echo "Test 02"
# sleep 1
# ./dhbwcc ../testing/tests/success/t03.c
# echo "Test 03"
# sleep 1
# ./dhbwcc ../testing/tests/success/t04.c
# echo "Test 04"
# sleep 1
# ./dhbwcc ../testing/tests/success/t05.c
# echo "Test 05"
# sleep 1
# ./dhbwcc ../testing/tests/success/t06.c
# echo "Test 06"
# sleep 1
# ./dhbwcc ../testing/tests/success/t07.c
# echo "Test 07"
# sleep 1
# ./dhbwcc ../testing/tests/success/t08.c
# echo "Test 08"
# sleep 1
# ./dhbwcc ../testing/tests/success/t09.c
# echo "Test 09"
# sleep 1
# ./dhbwcc ../testing/tests/success/t10.c
# echo "Test 10"
# sleep 1
# ./dhbwcc ../testing/tests/success/t11.c
# echo "Test 11"
# sleep 1
# ./dhbwcc ../testing/tests/success/t12.c
# echo "Test 12"