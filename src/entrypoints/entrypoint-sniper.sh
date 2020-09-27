#!/bin/bash

echo "[NVMSIM][INFO ] Sniper Docker entrypoint"
cd /opt/sniper && make
/opt/sniper/run-sniper -d /nvmsim/logs -- /bin/ls
