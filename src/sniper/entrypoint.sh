#!/bin/bash

echo "[NVMSIM] entrypoint.sh -> starting SNIPER Docker entrypoint"
cd /opt/sniper-7.2;
/opt/sniper-7.2/run-sniper -d /nvmsim/logs -- /bin/ls
echo "[NVMSIM] entrypoint.sh -> finishing SNIPER Docker entrypoint"
