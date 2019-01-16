#!/bin/bash

echo 'export PIN_HOME=/opt/pinplay-drdebug-3.5-pin-3.5-97503-gac534ca30-gcc-linux' >> ~/.bashrc
echo 'export SNIPER_HOME=/opt/sniper-7.1' >> ~/.bashrc
echo 'alias run-sniper=/opt/sniper-7.1/run-sniper' >> ~/.bashrc
source ~/.bashrc
cd /opt/sniper-7.1/; make

#todo: instalar benchmarks
#echo 'instalando benchmarks'
#tar -xf sniper-benchmarks.tbz --directory /opt
#echo 'export GRAPHITE_ROOT=$SNIPER_HOME'  >> ~/.bashrc
#echo 'export BENCHMARKS_ROOT=/opt/benchmarks' >> ~/.bashrc
#source ~/.bashrc
#echo 'testando a instalação do benchmarks'
#cd $BENCHMARKS_ROOT; make; ./run-sniper -p splash2-fft -i test -n 4 -c gainestown