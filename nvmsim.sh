#!/bin/bash

action=$1
option=$2
path=$(pwd)

# enter in project folder
cd $path

source ${path}/.env
sniper_path="${SNIPER_PATH}"
pinplay_path="${PINPLAY_PATH}"

function move_files() {
  if [ $sniper_path ] && [ $pinplay_path ]; then
    cp -r ${sniper_path} ${path}/vendor/sniper
    cp -r ${pinplay_path} ${path}/vendor/pinplay
  else
    echo "[NVMSIM][ERROR] You must define SNIPER_PATH amd PINPLAY_PATH in your .env file"
    exit 1
  fi
}

if [ $action ]; then
  if [ $action == build ]; then
    move_files
    docker-compose down
    docker-compose build $option
  elif [ $action == run ]; then
    docker-compose down && docker-compose up -d && docker-compose logs -f
  elif [ $action == stop ]; then
    docker-compose down
  else
    echo "[NVMSIM][ERROR] Parameter \"${action}\" not accepted. Please, use run, stop or build."
    exit 1
  fi
else
  docker-compose down && docker-compose up -d && docker-compose logs -f
fi
