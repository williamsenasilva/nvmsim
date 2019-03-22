#!/bin/bash

DATE=`date '+%Y-%m-%d-%Hh%Mm%Ss'`

cd /opt/spec_cpu2006/
. ./shrc 
cd /nvmsim/ 
mkdir -p ./logs/sniper/spec/$DATE 
mkdir -p ./logs/sniper/spec/$DATE/logs

echo "running..."
run-sniper -d ./logs/sniper/spec/$DATE/logs -- /opt/spec_cpu2006/bin/runspec --config=linux64-amd64-gcc63.cfg --test
echo "running... done"
