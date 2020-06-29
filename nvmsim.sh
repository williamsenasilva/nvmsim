#!/bin/bash 

action=$1
option=$2
path=$(pwd)

# enter in project folder
cd $path;

if [ $action ] ; then
    if [ $action == build ]; then
        docker-compose down 
        docker-compose build $option
    elif [ $action == run ]; then
        docker-compose down && docker-compose up -d && docker-compose logs -f
    elif [ $action == stop ]; then
        docker-compose down
    else
        echo "nvmsim: parameter not accepted. Please, use run, stop or build."  
    fi
else 
    docker-compose down && docker-compose up -d && docker-compose logs -f
fi
