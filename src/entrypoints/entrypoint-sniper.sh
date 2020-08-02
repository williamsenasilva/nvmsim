#!/bin/bash

echo "[NVMSIM][INFO] starting SNIPER Docker entrypoint"
cp /opt/sniper/run-sniper /mnt/shared/bin
cd /opt/sniper;
/opt/sniper/run-sniper -d /nvmsim/logs -- /bin/ls
echo "[NVMSIM][INFO ] finishing SNIPER Docker entrypoint"
