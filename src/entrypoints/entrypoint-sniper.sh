#!/bin/bash

echo "[NVMSIM][INFO ] Sniper Docker entrypoint"
cd /opt/sniper || exit 0

# fix pinplay libs
echo /opt/sniper/pin_kit/intel64/runtime/pincrt/ >> /etc/ld.so.conf.d/pin.conf
echo /opt/sniper/pin_kit/intel64/runtime/pincrt >> /etc/ld.so.conf.d/pin.conf
echo /opt/sniper/pin_kit/extras/xed-ia32/lib/libxed.so >> /etc/ld.so.conf.d/pin.conf
echo /opt/sniper/pin_kit/extras/xed-intel64/lib/libxed.so >> /etc/ld.so.conf.d/pin.conf
echo /opt/sniper/pin_kit/extras/xed-ia32/lib/ >> /etc/ld.so.conf.d/pin.conf
echo /opt/sniper/pin_kit/extras/xed-intel64/ >> /etc/ld.so.conf.d/pin.conf
echo /opt/sniper/xed_kit/lib/ >> /etc/ld.so.conf.d/pin.conf
# fix ldd lib not found in ldd pin_frontend
echo /opt/sniper/pin_kit/ia32/lib-ext/ >> /etc/ld.so.conf.d/pin.conf
echo /opt/sniper/pin_kit/intel64/lib-ext/ >> /etc/ld.so.conf.d/pin.conf
# fix error while loading shared libraries: libpython2.7.so.1.0 after run /opt/sniper/lib/sniper with frontend
echo /opt/sniper/python_kit/intel64/lib/ >> /etc/ld.so.conf.d/pin.conf
ldconfig

echo "[NVMSIM][INFO ] LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}"
#make clean
make -j $(nproc)
/opt/sniper/nvmsim
/opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim.cfg -- /opt/sniper/nvmsim
#/opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim.cfg --frontend /opt/sniper/frontend/pin-frontend/obj-intel64/pin_frontend -- /opt/sniper/nvmsim
#/opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim.cfg --frontend /opt/sniper/pin_kit/intel64/bin/pinbin -- /opt/sniper/nvmsim
tail -f /dev/null
