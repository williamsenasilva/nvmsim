#!/bin/bash

DATE=`date '+%Y-%m-%d-%Hh%Mm%Ss'`

for filepath in ./c_programs/memory_issues/*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    mkdir -p ./logs/sniper/memory_issues/$IMAGE/$DATE/$pathname 
    mkdir -p ./logs/sniper/memory_issues/$IMAGE/$DATE/$pathname/logs
done

gcc -Wall -O0 c_programs/memory_issues/01.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/01/01
gcc -Wall -O0 c_programs/memory_issues/01-a.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/01-a/01-a
gcc -Wall -O0 c_programs/memory_issues/02.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/02/02
gcc -Wall -O0 c_programs/memory_issues/02-a.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/02-a/02-a
gcc -Wall -O0 c_programs/memory_issues/03.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/03/03 -lpthread
gcc -Wall -O0 c_programs/memory_issues/03-a.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/03-a/03-a -lpthread
gcc -Wall -O3 c_programs/memory_issues/04.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/04/04
gcc -Wall -O3 c_programs/memory_issues/04-a.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/04-a/04-a
gcc -Wall -O3 -fno-tree-vectorize c_programs/memory_issues/05.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/05/05
gcc -Wall -O3 -fno-tree-vectorize c_programs/memory_issues/05-a.c -o ./logs/sniper/memory_issues/$IMAGE/$DATE/05-a/05-a

for filepath in ./c_programs/memory_issues/*.c ; do
    filename=$(basename $filepath)
    pathname="${filename%.c}"
    echo "running " $pathname " ..."
    run-sniper -d ./logs/sniper/memory_issues/$IMAGE/$DATE/$pathname/logs -- ./logs/sniper/memory_issues/$IMAGE/$DATE/$pathname/$pathname
    echo "running " $pathname " ... done"
done
