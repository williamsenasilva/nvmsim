#!/bin/bash

rm -rf ./note-tests &
BACK_PID=$!
wait $BACK_PID

for filepath in ./*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    mkdir -p ./note-tests/$pathname &
    BACK_PID=$!
    wait $BACK_PID
    mkdir -p ./note-tests/$pathname/logs &
    BACK_PID=$!
    wait $BACK_PID
done

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
    ./note-tests/$pathname/$pathname & 
    BACK_PID=$!
    wait $BACK_PID
    echo "finished " $pathname
done

# todo
# instalar valgrind