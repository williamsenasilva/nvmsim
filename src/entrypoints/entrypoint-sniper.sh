#!/bin/bash

echo "[NVMSIM][INFO ] Sniper Docker entrypoint"
cd /opt/sniper || exit 0
make -j $(nproc)
/opt/sniper/nvmsim
/opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim.cfg -- /opt/sniper/nvmsim
#/opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim.cfg -- /bin/ls
