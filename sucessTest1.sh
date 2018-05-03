# build dhbwcc
rm -r build && mkdir build && cd build && cmake .. && make

#Feel free to change the tests you want to look at
#This file is going to be on the .gitIgnore list
./dhbwcc ../testing/tests/success/t04.c
./dhbwcc ../testing/tests/success/t11.c
./dhbwcc ../testing/tests/success/t03.c