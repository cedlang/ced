#!/usr/bin/bash

red="\033[0;31m"; green="\033[0;32m"; reset="\033[0m"
err=0; ok=0

test() {
    echo -e Testing $1...
    gcc $1.c ../src/{$1,str}.c -o $1

    if ./$1
    then
        ((ok+=1))
    else
        ((err+=1))
    fi
    rm $1
}

test lexer

echo -e ${green}$ok PASSED ${red}$err FAILED${reset}
