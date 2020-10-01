#!/bin/bash

echo "[NVMSIM][INFO ] Sniper Docker entrypoint"
cd /opt/sniper || exit 0
make
/opt/sniper/run-sniper -d /mnt/nvmsim/logs -- /bin/ls
