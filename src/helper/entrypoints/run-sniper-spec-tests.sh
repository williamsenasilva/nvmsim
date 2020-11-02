#!/bin/bash

DATE=`date '+%Y-%m-%d-%Hh%Mm%Ss'`

cd /opt/spec_cpu2006/ 
. ./shrc 
cd /nvmsim/ 
mkdir -p ./logs/sniper/spec/$DATE 
mkdir -p ./logs/sniper/spec/$DATE/logs

echo "running..."
run-sniper -d ./logs/sniper/spec/$DATE/logs -- /opt/spec_cpu2006/bin/runspec --config=linux64-amd64-gcc73.cfg --size=test --tune=base --noreportable --iterations=1 bzip2
echo "running... done"
