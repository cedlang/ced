#!/usr/bin/bash

rm -f ced
gcc $(find src -name "*.c") -O3 -s -Wall -Wextra -Werror -std=c99 -o ced

case "$1" in
    "run")
        ./ced $2
        ;;

    "test")
        pushd test
            ./test.sh
        popd
        ;;
esac
