#!/bin/bash

DATE=`date '+%Y-%m-%d-%Hh%Mm%Ss'`

for filepath in ./memory-tests/*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    mkdir -p ./docker-tests/memory-tests/$DATE/$pathname 
    mkdir -p ./docker-tests/memory-tests/$DATE/$pathname/logs
done

gcc -Wall -O0 memory-tests/01.c -o ./docker-tests/memory-tests/$DATE/01/01
gcc -Wall -O0 memory-tests/01-a.c -o ./docker-tests/memory-tests/$DATE/01-a/01-a
gcc -Wall -O0 memory-tests/02.c -o ./docker-tests/memory-tests/$DATE/02/02
gcc -Wall -O0 memory-tests/02-a.c -o ./docker-tests/memory-tests/$DATE/02-a/02-a
gcc -Wall -O0 memory-tests/03.c -o ./docker-tests/memory-tests/$DATE/03/03 -lpthread
gcc -Wall -O0 memory-tests/03-a.c -o ./docker-tests/memory-tests/$DATE/03-a/03-a -lpthread
gcc -Wall -O3 memory-tests/04.c -o ./docker-tests/memory-tests/$DATE/04/04
gcc -Wall -O3 memory-tests/04-a.c -o ./docker-tests/memory-tests/$DATE/04-a/04-a
gcc -Wall -O3 -fno-tree-vectorize memory-tests/05.c -o ./docker-tests/memory-tests/$DATE/05/05
gcc -Wall -O3 -fno-tree-vectorize memory-tests/05-a.c -o ./docker-tests/memory-tests/$DATE/05-a/05-a

for filepath in ./memory-tests/*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    valgrind --leak-check=yes --log-file=./docker-tests/memory-tests/$DATE/$pathname/logs/valgrind.log ./docker-tests/memory-tests/$DATE/$pathname/$pathname
    echo "finished " $pathname
done
