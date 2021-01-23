#!/bin/bash

echo "[NVMSIM][INFO ] Sniper Docker entrypoint"

ENABLE_SNIPER=${ENABLE_SNIPER:-1}

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW="\033[0;33m"
NOCOLOR='\033[0m'

if [ $ENABLE_SNIPER == 1 ]; then
    cd /opt/sniper || exit 0
    make -j $(nproc)
    /opt/sniper/nvmsim
    /opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim.cfg -- /opt/sniper/nvmsim
    #/opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim.cfg -- /bin/ls
else
    echo -e "[NVMSIM][${YELLOW}WARN ${NOCOLOR}] Sniper Docker entrypoint disabled"
    tail -f /dev/null
fi
