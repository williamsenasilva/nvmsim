#!/bin/bash

echo "[NVMSIM][INFO ] NVMain Docker entrypoint"
cd /opt/nvmain && scons --build-type=fast
#scons --build-type=debug
#scons --build-type=prof
./nvmain.fast Config/2D_DRAM_example.config /mnt/nvmsim/tracefile 0 TraceReader=SniperTrace
