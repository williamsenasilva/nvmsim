#!/bin/bash

ENABLE_SNIPER=${ENABLE_SNIPER:-1}
SNIPER_TARGET_ARCH=${SNIPER_TARGET_ARCH:-intel64}
SNIPER_MEMORY_TYPE=${SNIPER_MEMORY_TYPE:-RAM}
NVMSIM_NVM_CONFIG_FILE=${NVMSIM_NVM_CONFIG_FILE-:2D_DRAM_example.config}
INSTANCE_INDEX=1

source /nvmsim/scripts/util.sh

get_container_number() {
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
  log_debg "I am instance $INSTANCE_INDEX"
}

set_timezone() {
  command="ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone"
  run_command "$command"
  log_info "timezone: $TZ"
}

check_sniper_requirements() {
  if [ $ENABLE_SNIPER != 1 ]; then
    log_warn "Sniper Docker entrypoint disabled."
    tail -f /dev/null
  fi
}

run_sniper() {
  cd /opt/sniper || exit 0

  cd /opt/sniper || exit 0

  get_container_number
    
  # TODO: read from environment value
  program_name="test_mmap"

  if [ $COUNT == 0 ] || [ $INSTANCE_INDEX == 1 ]; then
    command="make -j $(nproc)"
    run_command "$command"
    touch /mnt/nvmsim/sniper-ready
    
    command="gcc /mnt/nvmsim/src/${program_name}.c -g -O0 -o /opt/sniper/${program_name}"
    run_command "$command"
  fi

  while [ ! -f /mnt/nvmsim/sniper-ready ]; do 
    log_info "Sniper main container is busy, wait 10s."
    sleep 10s
  done

  if [ $SNIPER_MEMORY_TYPE == NVM ]; then
    IFS=', ' read -r -a nvmain_config_files <<< "${NVMAIN_CONFIG_FILES}"
    if [ $COUNT == 0 ]; then
      array_index=0
      command="export NVMSIM_TRACEFILE_PATH=/mnt/nvmsim/tracefile-instance-1"
    else
      array_index=$(expr $INSTANCE_INDEX - 1)
      command="export NVMSIM_TRACEFILE_PATH=/mnt/nvmsim/tracefile-instance-${INSTANCE_INDEX}"
    fi
    nvmain_config_file=${nvmain_config_files[$array_index]}
    run_command "$command"
  fi

  command="/opt/sniper/run-sniper -d /mnt/nvmsim/logs -c /opt/sniper/config/nvmsim-nvm.cfg -- /opt/sniper/${program_name} 1 0 /tmp/pagefile"
  run_command "$command"
}

run_sniper_with_speccpu_commands() {
  cd /opt/sniper || exit 0

  get_container_number
    
  if [ $INSTANCE_INDEX == 1 ]; then
    make -j $(nproc)
    touch /mnt/nvmsim/sniper-ready
  fi

  while [ ! -f /mnt/nvmsim/sniper-ready ]; do 
    log_info "Sniper main container is busy, wait 10s."
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

  log_info "Getting speccpu commands."
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
      log_done "${benchmark}"
      count=1
      cat ${file_to_check} | grep "${benchmark_binary}" | while read line; do
        benchmark_command="/mnt/nvmsim/speccpu/${benchmark}/${benchmark_binary}"
        benchmark_command="$benchmark_command"$(echo $line | awk -F ${benchmark_binary} '{print $2}')
        cd /mnt/nvmsim/speccpu/${benchmark} || exit 0

        current_date_time="`date +%Y-%m-%d-%Hh%Mm%Ss`"

        if [ $SNIPER_MEMORY_TYPE == NVM ]; then
          
          IFS=', ' read -r -a nvmain_config_files <<< "${NVMAIN_CONFIG_FILES}"
          array_index=$(expr $INSTANCE_INDEX - 1)
          nvmain_config_file=${nvmain_config_files[$array_index]}
          command="export NVMSIM_TRACEFILE_PATH=/mnt/nvmsim/tracefile-instance-${INSTANCE_INDEX}"
          run_command "$command"

          # put sniper on scale mode
          if [ $nvmain_config_file != SNIPER ]; then
            command="/opt/sniper/run-sniper -d /mnt/nvmsim/simulations/${benchmark}/instance-${INSTANCE_INDEX}-${nvmain_config_file}-test-${count}-${current_date_time} -c /opt/sniper/config/nvmsim-nvm.cfg -- ${benchmark_command}"
            log_info "Benchmark ${benchmark} (${count}) started. command: ${command}"
          else 
            command="/opt/sniper/run-sniper -d /mnt/nvmsim/simulations/${benchmark}/instance-${INSTANCE_INDEX}-${nvmain_config_file}-test-${count}-${current_date_time} -c /opt/sniper/config/nvmsim-ram.cfg -- ${benchmark_command}"
            log_info "Benchmark ${benchmark} (${count}) started. command: ${command}"  
          fi

          run_command "$command"
        elif [ $SNIPER_MEMORY_TYPE == RAM ]; then
          command="/opt/sniper/run-sniper -d /mnt/nvmsim/simulations/${benchmark}/sniper-DRAM-test-${count}-${current_date_time} -c /opt/sniper/config/nvmsim-ram.cfg -- ${benchmark_command}"
          log_info "Benchmark ${benchmark} (${count}) started. command: ${command}"
          run_command "$command"
        else
          log_errr "Memory \"$SNIPER_MEMORY_TYPE\"not accepted. Usage: [NVM, RAM]"
          exit 0
        fi
        log_done "Benchmark ${benchmark} (${count}) finished."

        count=$((count + 1))
      done
    else
      log_errr "${file_to_check} not found."
    fi
  done
  log_info "Getting speccpu commands."
}

run() {
  log_info "Sniper Docker entrypoint started"
  
  check_sniper_requirements
  set_timezone
  run_sniper
  # run_sniper_with_speccpu_commands
  
  log_info "Sniper Docker entrypoint finished"
  tail -f /dev/null
}

run