#!/bin/bash

echo "[NVMSIM][INFO ] starting NVMAIN Docker entrypoint"
cp /opt/nvmain/nvmain.fast /mnt/shared/bin
cp /opt/nvmain/nvmain.debug /mnt/shared/bin
cp /opt/nvmain/nvmain.prof /mnt/shared/bin
cd /opt/nvmain;
./nvmain.fast Config/2D_DRAM_example.config /mnt/shared/nvmsim-tracefile 0 TraceReader=SniperTrace
echo "[NVMSIM][INFO ] finishing NVMAIN Docker entrypoint"