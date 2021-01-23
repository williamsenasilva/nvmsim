#!/bin/bash

echo "[NVMSIM][INFO ] NVMain Docker entrypoint"

ENABLE_NVMAIN=${ENABLE_NVMAIN:-1}

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW="\033[0;33m"
NOCOLOR='\033[0m'

if [ $ENABLE_NVMAIN == 1 ]; then
    cd /opt/nvmain || exit 0
    scons --build-type=fast
    scons --build-type=prof
    scons --build-type=debug
    ./nvmain.debug Config/2D_DRAM_example.config /mnt/nvmsim/tracefile 0 TraceReader=SniperTrace
else
    echo -e "[NVMSIM][${YELLOW}WARN ${NOCOLOR}] NVMain Docker entrypoint disabled"
    tail -f /dev/null
fi