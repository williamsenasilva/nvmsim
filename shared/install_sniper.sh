#!/bin/bash

echo 'instalando pinplay'
tar -xf /nvmsim/shared/vendor/pinplay-drdebug-3.5-pin-3.5-97503-gac534ca30-gcc-linux.tar.bz2 --directory /opt
echo 'export PIN_HOME=/opt/pinplay-drdebug-3.5-pin-3.5-97503-gac534ca30-gcc-linux' >> ~/.bashrc
source ~/.bashrc

echo 'instalando sniper'
tar -xf /nvmsim/shared/vendor/sniper-latest.tgz --directory /opt
echo 'export SNIPER_HOME=/opt/sniper-7.1' >> ~/.bashrc
source ~/.bashrc
cd /opt/sniper-7.1/; make

alias run-sniper=/opt/sniper-7.1/run-sniper

echo 'testando a instalação do sniper'
cd /opt/sniper-7.1/test/fft; make run
#todo: instalar benchmarks
#echo 'instalando benchmarks'
#tar -xf sniper-benchmarks.tbz --directory /opt
#echo 'export GRAPHITE_ROOT=$SNIPER_HOME'  >> ~/.bashrc
#echo 'export BENCHMARKS_ROOT=/opt/benchmarks' >> ~/.bashrc
#source ~/.bashrc
#echo 'testando a instalação do benchmarks'
#cd $BENCHMARKS_ROOT; make; ./run-sniper -p splash2-fft -i test -n 4 -c gainestown