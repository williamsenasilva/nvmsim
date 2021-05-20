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

function get_container_number
{
  # from: https://stackoverflow.com/questions/60480257/how-to-simply-scale-a-docker-compose-service-and-pass-the-index-and-count-to-eac

  # get the container IP
  IP=`ifconfig eth0 | grep 'inet ' | awk '{print $2}'`

  # get the service name you specified in the docker-compose.yml 
  # by a reverse DNS lookup on the IP
  SERVICE=`dig -x $IP +short | cut -d'_' -f2`

  # the number of replicas is equal to the A records 
  # associated with the service name
  COUNT=`dig $SERVICE +short | wc -l`

  # extract the replica number from the same PTR entry
  INSTANCE_INDEX=`dig -x $IP +short | sed 's/.*_\([0-9]*\)\..*/\1/'`
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

  get_container_number
    
  if [ $INSTANCE_INDEX == 1 ]; then
    make -j $(nproc)
    touch /mnt/nvmsim/sniper-ready
  fi

  while [ ! -f /mnt/nvmsim/sniper-ready ]; do 
    message="Sniper main container is busy, wait 10s."
    echo "${message_info}${message_action} ${message}"
    sleep 10s
  done
  
  benchmarks=(
    999.specrand
    998.specrand
    483.xalancbmk
    482.sphinx3
    481.wrf
    473.astar
    471.omnetpp
    470.lbm
    465.tonto
    464.h264ref
    462.libquantum
    459.GemsFDTD
    458.sjeng
    456.hmmer
    454.calculix
    453.povray
    450.soplex
    447.dealII
    445.gobmk
    444.namd
    437.leslie3d
    436.cactusADM
    435.gromacs
    434.zeusmp
    433.milc
    429.mcf
    416.gamess
    410.bwaves
    403.gcc
    401.bzip2
    400.perlbench
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

        current_date_time="`date +%Y-%m-%d-%Hh%Mm%Ss`"

        if [ $SNIPER_MEMORY_TYPE == NVM ]; then
          
          IFS=', ' read -r -a nvmain_config_files <<< "${NVMAIN_CONFIG_FILES}"
          array_index=$(expr $INSTANCE_INDEX - 1)
          nvmain_config_file=${nvmain_config_files[$array_index]}
          command="export NVMSIM_TRACEFILE_PATH=/mnt/nvmsim/tracefile-instance-${INSTANCE_INDEX}"
          message="command: $command"
          echo "${message_info}${message_action} ${message}"
          eval $command

          # put sniper on scale mode
          if [ $nvmain_config_file != SNIPER ]; then
            command="/opt/sniper/run-sniper -d /mnt/nvmsim/simulations/${benchmark}/instance-${INSTANCE_INDEX}-${nvmain_config_file}-test-${count}-${current_date_time} -c /opt/sniper/config/nvmsim-nvm.cfg -- ${benchmark_command}"
            message="Benchmark ${benchmark} (${count}) started. command: ${command}"
          else 
            command="/opt/sniper/run-sniper -d /mnt/nvmsim/simulations/${benchmark}/instance-${INSTANCE_INDEX}-${nvmain_config_file}-test-${count}-${current_date_time} -c /opt/sniper/config/nvmsim-ram.cfg -- ${benchmark_command}"
            message="Benchmark ${benchmark} (${count}) started. command: ${command}"  
          fi

          echo "${message_info}${message_action} ${message}"
          eval $command
        elif [ $SNIPER_MEMORY_TYPE == RAM ]; then
          command="/opt/sniper/run-sniper -d /mnt/nvmsim/simulations/${benchmark}/sniper-DRAM-test-${count}-${current_date_time} -c /opt/sniper/config/nvmsim-ram.cfg -- ${benchmark_command}"
          message="Benchmark ${benchmark} (${count}) started. command: ${command}"
          echo "${message_info}${message_action} ${message}"
          eval $command
        else
          message="Memory \"$SNIPER_MEMORY_TYPE\"not accepted. Usage: [NVM, RAM]"
          echo -e "${message_erro}${message_action} ${message}"
          exit 0
        fi
        message="Benchmark ${benchmark} (${count}) finished."
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
}

run