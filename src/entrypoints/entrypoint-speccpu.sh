#!/bin/bash

ENABLE_SPECCPU=${ENABLE_SPECCPU:-1}

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW="\033[0;33m"
NOCOLOR='\033[0m'

message_info="[NVMSIM][INFO ]"
message_warn="[NVMSIM][${YELLOW}WARN${NOCOLOR} ]"
message_erro="[NVMSIM][${RED}ERROR${NOCOLOR}]"
message_done="[NVMSIM][${GREEN}DONE${NOCOLOR} ]"

function run
{
  message_action="[RUNEP]"
  message="SpecCPU Docker entrypoint started."
  echo "${message_info}${message_action} ${message}"

  check_speccpu_requirements
  install_speccpu
  copy_speccpu_container_folder_to_host_speccpu_folder
  
  message_action="[RUNEP]"
  message="SpecCPU Docker entrypoint finished."
  echo "${message_info}${message_action} ${message}"
  tail -f /dev/null
}

function check_speccpu_requirements
{
  if [ $ENABLE_SPECCPU == 1 ]; then
    if [ ! -f "/mnt/dvd/install.sh" ]; then
      message="File install.sh not found. Check if dvd volume was mounted."
      echo -e "${message_erro}${message_action} ${message}"
      exit 0
    fi
  else
    message="SpecCPU Docker entrypoint disabled."
    echo -e "${message_warn}${message_action} ${message}"
    tail -f /dev/null
  fi
}

function install_speccpu
{
  message_action="[SETUP]"
  message="Installing speccpu on path /otp/speccpu"
  echo "${message_info}${message_action} ${message}"
  cd /mnt/dvd || exit 0
  yes | ./install.sh -d /opt/speccpu
  cd /opt/speccpu || exit 0
  message="Instalation finished."
  echo -e "${message_done}${message_action} ${message}"

  source ./shrc
  cp $SPEC/config/Example-linux64-amd64-gcc43+.cfg $SPEC/config/nvmsim.cfg

  message_action="[BUILD]"
  message="runspec started."
  echo -e "${message_info}${message_action} ${message}"
  runspec --action=run --verbose=99 --size=test --config=nvmsim.cfg all
  message="runspec finished."
  echo -e "${message_done}${message_action} ${message}"

  benchmarks=(
    400.perlbench
    401.bzip2
    403.gcc
    410.bwaves
    416.gamess
    429.mcf
    433.milc
    434.zeusmp
    435.gromacs
    436.cactusADM
    437.leslie3d
    444.namd
    445.gobmk
    447.dealII
    450.soplex
    453.povray
    454.calculix
    456.hmmer
    458.sjeng
    459.GemsFDTD
    462.libquantum
    464.h264ref
    465.tonto
    470.lbm
    471.omnetpp
    473.astar
    481.wrf
    482.sphinx3
    483.xalancbmk
    998.specrand
    999.specrand
  )

  rm -rf /mnt/nvmsim/speccpu
  mkdir -p /mnt/nvmsim/speccpu

  message_action="[CHECK]"
  for benchmark in "${benchmarks[@]}"; do

    benchmark_binary=$(cut -d "." -f2 <<< "${benchmark}")
    benchmark_binary="${benchmark_binary}_base.gcc43-64bit"
    if [ ${benchmark} == "482.sphinx3" ]; then
      benchmark_binary="sphinx_livepretend_base.gcc43-64bit"
    elif [ ${benchmark} == "483.xalancbmk" ]; then
      benchmark_binary="Xalan_base.gcc43-64bit"
    fi

    file_to_check="/opt/speccpu/benchspec/CPU2006/${benchmark}/run/run_base_test_gcc43-64bit.0000/${benchmark_binary}"
    if [ -f "${file_to_check}" ]; then
      mkdir -p /mnt/nvmsim/speccpu/${benchmark}
      cp -r /opt/speccpu/benchspec/CPU2006/"${benchmark}"/run/run_base_test_gcc43-64bit.0000/* /mnt/nvmsim/speccpu/${benchmark}/
      message="${benchmark}"
      echo -e "${message_done}${message_action} ${message}"
    else
      message="Binary file ${benchmark_binary} not found."
      echo -e "${message_erro}${message_action} ${message}"
    fi
  done
}

# todo: Fix this copy. I need to copy because when the shared folder is used as /opt/speccpu the installation fails
function copy_speccpu_container_folder_to_host_speccpu_folder
{
  message_action="[COPY ]"
  message="Copying /opt/speccpu to host shared folder..."
  echo -n "${message_info}${message_action} ${message}"
  rm -rf /mnt/speccpu/*
  cp -r /opt/speccpu/* /mnt/speccpu/
  echo "ok"
}

run