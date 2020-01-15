#!/bin/bash

echo "[NVMSIM] entrypoint.sh -> starting NVMAIN Docker entrypoint"
cp /opt/nvmain/nvmain.fast /mnt/shared/bin
cp /opt/nvmain/nvmain.debug /mnt/shared/bin
cp /opt/nvmain/nvmain.prof /mnt/shared/bin
cd /opt/nvmain;
./nvmain.fast Config/3D_DRAM_example.config tracefile.nvt 0 TraceReader=SniperTrace
echo "[NVMSIM] entrypoint.sh -> finishing NVMAIN Docker entrypoint"