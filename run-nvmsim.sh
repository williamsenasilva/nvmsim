#!/bin/bash 

# enter in project folder
path=$(pwd)
cd $path;

# update project
git pull origin master

# docker-compose actions
docker-compose pull
docker-compose down && docker-compose up -d && docker-compose logs -f -t
