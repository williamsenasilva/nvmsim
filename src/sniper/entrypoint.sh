#!/bin/bash

echo "[NVMSIM] entrypoint.sh -> starting SNIPER Docker entrypoint"
cp /opt/sniper-7.2/run-sniper /mnt/shared/bin
cd /opt/sniper-7.2;
/opt/sniper-7.2/run-sniper -d /nvmsim/logs -- /bin/ls
echo "[NVMSIM] entrypoint.sh -> finishing SNIPER Docker entrypoint"
