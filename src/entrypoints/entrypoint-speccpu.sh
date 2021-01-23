#!/bin/bash

echo "[NVMSIM][INFO ] SPEC CPU Docker entrypoint"

ENABLE_SPECCPU=${ENABLE_SPECCPU:-1}

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW="\033[0;33m"
NOCOLOR='\033[0m'

if [ $ENABLE_SPECCPU == 1 ]; then
    if [ ! -f "/mnt/dvd/install.sh" ]; then
        echo -e "[NVMSIM][${RED}ERROR${NOCOLOR}] File install.sh not found. Check if dvd volume was mounted."
        exit 0
    else
        yes | ./install.sh -d /opt/speccpu
        cd /opt/speccpu || exit 0
        . ./shrc
        cd $SPEC/config
        cp Example-linux64-amd64-gcc43+.cfg mytest.cfg
        runspec --config=mytest.cfg --action=build --tune=base bzip2
    fi
else
    echo -e "[NVMSIM][${YELLOW}WARN ${NOCOLOR}] SPEC CPU Docker entrypoint disabled"
    tail -f /dev/null
fi