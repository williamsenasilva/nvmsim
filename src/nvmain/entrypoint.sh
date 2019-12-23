#!/bin/bash

echo "[NVMSIM] entrypoint.sh -> starting NVMAIN Docker entrypoint"
cd /opt/nvmain;
scons --build-type=fast
# ./nvmain.fast Config/3D_DRAM_example.config tracefile.nvt 1000 TraceReader=SniperTrace
./nvmain.fast Config/3D_DRAM_example.config new-tracefile.nvt 1000 TraceReader=SniperTrace
# ./nvmain.fast 
echo "[NVMSIM] entrypoint.sh -> finishing NVMAIN Docker entrypoint"