#!/bin/bash

echo "[NVMSIM] entrypoint.sh -> starting NVMAIN Docker entrypoint"
cd /opt/nvmain;
# scons --build-type=fast
./nvmain.fast Config/2D_DRAM_example.config tracefile.nvt 0
echo "[NVMSIM] entrypoint.sh -> finishing NVMAIN Docker entrypoint"