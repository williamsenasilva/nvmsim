#!/bin/bash

rm -rf ./sniper-tests

for filepath in ./*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    mkdir -p ./sniper-tests/$pathname
    mkdir -p ./sniper-tests/$pathname/logs
done

gcc -Wall -O0 hello_world.c -o ./sniper-tests/hello_world/hello_world
gcc -Wall -O0 01.c -o ./sniper-tests/01/01
gcc -Wall -O0 01-a.c -o ./sniper-tests/01-a/01-a
gcc -Wall -O0 02.c -o ./sniper-tests/02/02
gcc -Wall -O0 02-a.c -o ./sniper-tests/02-a/02-a
gcc -Wall -O0 03.c -o ./sniper-tests/03/03 -lpthread
gcc -Wall -O0 03-a.c -o ./sniper-tests/03-a/03-a -lpthread
gcc -Wall -O3 04.c -o ./sniper-tests/04/04
gcc -Wall -O3 04-a.c -o ./sniper-tests/04-a/04-a
gcc -Wall -O3 -fno-tree-vectorize 05.c -o ./sniper-tests/05/05
gcc -Wall -O3 -fno-tree-vectorize 05-a.c -o ./sniper-tests/05-a/05-a

alias run-sniper=/opt/sniper-7.1/run-sniper

for filepath in ./*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    run-sniper -d ./sniper-tests/$pathname/logs -- ./sniper-tests/$pathname/$pathname
done