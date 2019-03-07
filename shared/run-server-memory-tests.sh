#!/bin/bash

DATE=`date '+%Y-%m-%d-%Hh%Mm%Ss'`

for filepath in ./c_programs/memory_issues/*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    mkdir -p ./logs/server/memory_issues/$DATE/$pathname 
    mkdir -p ./logs/server/memory_issues/$DATE/$pathname/logs
done

gcc -Wall -O0 c_programs/memory_issues/01.c -o ./logs/server/memory_issues/$DATE/01/01
gcc -Wall -O0 c_programs/memory_issues/01-a.c -o ./logs/server/memory_issues/$DATE/01-a/01-a
gcc -Wall -O0 c_programs/memory_issues/02.c -o ./logs/server/memory_issues/$DATE/02/02
gcc -Wall -O0 c_programs/memory_issues/02-a.c -o ./logs/server/memory_issues/$DATE/02-a/02-a
gcc -Wall -O0 c_programs/memory_issues/03.c -o ./logs/server/memory_issues/$DATE/03/03 -lpthread
gcc -Wall -O0 c_programs/memory_issues/03-a.c -o ./logs/server/memory_issues/$DATE/03-a/03-a -lpthread
gcc -Wall -O3 c_programs/memory_issues/04.c -o ./logs/server/memory_issues/$DATE/04/04
gcc -Wall -O3 c_programs/memory_issues/04-a.c -o ./logs/server/memory_issues/$DATE/04-a/04-a
gcc -Wall -O3 -fno-tree-vectorize c_programs/memory_issues/05.c -o ./logs/server/memory_issues/$DATE/05/05
gcc -Wall -O3 -fno-tree-vectorize c_programs/memory_issues/05-a.c -o ./logs/server/memory_issues/$DATE/05-a/05-a

for filepath in ./c_programs/memory_issues/*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    echo "running " $pathname " ..."
    valgrind --leak-check=yes --log-file=./logs/server/memory_issues/$DATE/$pathname/logs/valgrind.log ./logs/server/memory_issues/$DATE/$pathname/$pathname
    echo "running " $pathname " ... done"
done
