# build dhbwcc
rm -r build && mkdir build && cd build && cmake .. && make

#Feel free to change the tests you want to look at
#This file is going to be on the .gitIgnore list
./dhbwcc ../testing/tests/success/t01.c
./dhbwcc ../testing/tests/success/t02.c
./dhbwcc ../testing/tests/success/t03.c
./dhbwcc ../testing/tests/success/t04.c
./dhbwcc ../testing/tests/success/t05.c
./dhbwcc ../testing/tests/success/t06.c
./dhbwcc ../testing/tests/success/t07.c
./dhbwcc ../testing/tests/success/t08.c
./dhbwcc ../testing/tests/success/t09.c
./dhbwcc ../testing/tests/success/t10.c
./dhbwcc ../testing/tests/success/t11.c
./dhbwcc ../testing/tests/success/t12.c