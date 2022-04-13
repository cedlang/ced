#!/usr/bin/bash

rm -f ced
gcc $(find src -name "*.c") -O3 -s -Wall -Wextra -Werror -std=c99 -o ced

if [ "$1" == "run" ]
then
    ./ced $2
fi
