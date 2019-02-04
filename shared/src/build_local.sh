#!/bin/bash

rm -rf ./note-tests

for filepath in ./*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    mkdir -p ./note-tests/$pathname 
    mkdir -p ./note-tests/$pathname/logs
done

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )" 

gcc -Wall -O0 hello_world.c -o ./note-tests/hello_world/hello_world
gcc -Wall -O0 01.c -o ./note-tests/01/01
gcc -Wall -O0 01-a.c -o ./note-tests/01-a/01-a
gcc -Wall -O0 02.c -o ./note-tests/02/02
gcc -Wall -O0 02-a.c -o ./note-tests/02-a/02-a
gcc -Wall -O0 03.c -o ./note-tests/03/03 -lpthread
gcc -Wall -O0 03-a.c -o ./note-tests/03-a/03-a -lpthread
gcc -Wall -O3 04.c -o ./note-tests/04/04
gcc -Wall -O3 04-a.c -o ./note-tests/04-a/04-a
gcc -Wall -O3 -fno-tree-vectorize 05.c -o ./note-tests/05/05
gcc -Wall -O3 -fno-tree-vectorize 05-a.c -o ./note-tests/05-a/05-a

for filepath in ./*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    valgrind --leak-check=yes --log-file=./note-tests/$pathname/logs/valgrind.log ./note-tests/$pathname/$pathname
    echo "finished " $pathname
done


# todo
# instalar valgrind