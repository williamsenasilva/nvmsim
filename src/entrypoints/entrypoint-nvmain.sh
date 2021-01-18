#!/bin/bash

echo "[NVMSIM][INFO ] NVMain Docker entrypoint"
#tail -f /dev/null
cd /opt/nvmain || exit 0
scons --build-type=fast
scons --build-type=prof
scons --build-type=debug
./nvmain.debug Config/2D_DRAM_example.config /mnt/nvmsim/tracefile 0 TraceReader=SniperTrace
