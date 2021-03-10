#!/bin/bash

ENABLE_SNIPER=${ENABLE_SNIPER:-1}
SNIPER_TARGET_ARCH=${SNIPER_TARGET_ARCH:-intel64}
SNIPER_MEMORY_TYPE=${SNIPER_MEMORY_TYPE:-RAM}
NVMSIM_NVM_CONFIG_FILE=${NVMSIM_NVM_CONFIG_FILE-:2D_DRAM_example.config}

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
  message="Sniper Docker entrypoint started."
  echo "${message_info}${message_action} ${message}"
  
  check_sniper_requirements
  set_timezone
  # run_sniper
  run_sniper_with_speccpu_commands
  
  message_action="[RUNEP]"
  message="Sniper Docker entrypoint finished."
  echo "${message_info}${message_action} ${message}"
  tail -f /dev/null
}

function set_timezone
{
    message_action="[SETTZ]"
    message="Setting timezone... "
    echo -n "${message_info}${message_action} ${message}"
    ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
    echo "ok (${TZ})"
}

function check_sniper_requirements
{
  if [ $ENABLE_SNIPER != 1 ]; then
    message="Sniper Docker entrypoint disabled."
    echo -e "${message_warn}${message_action} ${message}"
    tail -f /dev/null
  fi
}

function run_sniper
{
  cd /opt/sniper || exit 0
  make -j $(nproc)
  /opt/sniper/run-sniper -d /mnt/nvmsim/logs -- /opt/sniper/nvmsim
}

function run_sniper_with_speccpu_commands
{
  cd /opt/sniper || exit 0
  make -j $(nproc)
  
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

  message_action="[GCMDS]"
  message="Getting speccpu commands."
  echo "${message_info}${message_action} ${message}"
  for benchmark in "${benchmarks[@]}"; do

    benchmark_binary=$(cut -d "." -f2 <<< "${benchmark}")
    benchmark_binary="${benchmark_binary}_base.gcc43-64bit"
    if [ ${benchmark} == "482.sphinx3" ]; then
      benchmark_binary="sphinx_livepretend_base.gcc43-64bit"
    elif [ ${benchmark} == "483.xalancbmk" ]; then
      benchmark_binary="Xalan_base.gcc43-64bit"
    fi

    file_to_check="/mnt/nvmsim/speccpu/${benchmark}/speccmds.cmd"
    if [ -f "${file_to_check}" ]; then
      message="${benchmark}"
      echo -e "${message_done}${message_action} ${message}"
      count=1
      cat ${file_to_check} | grep "${benchmark_binary}" | while read line; do
        benchmark_command="/mnt/nvmsim/speccpu/${benchmark}/${benchmark_binary}"
        benchmark_command="$benchmark_command"$(echo $line | awk -F ${benchmark_binary} '{print $2}')
        cd /mnt/nvmsim/speccpu/${benchmark} || exit 0
        message_action="[BMARK]"

        current_date_time="`date +%Y-%m-%d-%H-%M-%S`"

        if [ $SNIPER_MEMORY_TYPE = NVM ]; then
          command="/opt/sniper/run-sniper -d /mnt/nvmsim/speccpu/${benchmark}/nvmain-${NVMSIM_NVM_CONFIG_FILE}-test-${count}-${current_date_time}/logs -c /opt/sniper/config/nvmsim-nvm.cfg -- ${benchmark_command}"
          message="Benchmark ${benchmark} nvmain (${count}) started. command: ${command}"
          echo "${message_info}${message_action} ${message}"
          eval $command
        elif [ $SNIPER_MEMORY_TYPE = RAM ]; then
          command="/opt/sniper/run-sniper -d /mnt/nvmsim/speccpu/${benchmark}/sniper-DRAM-test-${count}-${current_date_time}/logs -c /opt/sniper/config/nvmsim-ram.cfg -- ${benchmark_command}"
          message="Benchmark ${benchmark} nvmain (${count}) started. command: ${command}"
          echo "${message_info}${message_action} ${message}"
          eval $command
        else
          message="Memory \"$SNIPER_MEMORY_TYPE\"not accepted. Usage: [NVM, RAM]"
          echo -e "${message_erro}${message_action} ${message}"
          exit 0
        fi
        message="Benchmark ${benchmark} nvmain (${count}) finished."
        echo -e "${message_done}${message_action} ${message}"

        count=$((count + 1))
      done
    else
      message="${file_to_check} not found."
      echo -e "${message_erro}${message_action} ${message}"
    fi
  done
  message_action="[GCMDS]"
  message="Getting speccpu commands."
  echo "${message_info}${message_action} ${message}"
}

run