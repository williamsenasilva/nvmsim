#!/bin/bash

echo "[NVMSIM] entrypoint.sh - starting SNIPER Docker entrypoint"
cp /opt/sniper-7.3/run-sniper /mnt/shared/bin
cd /opt/sniper-7.3;
/opt/sniper-7.3/run-sniper -d /nvmsim/logs -- /bin/ls
echo "[NVMSIM] entrypoint.sh - finishing SNIPER Docker entrypoint"
