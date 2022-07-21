#!/bin/bash

ENABLE_NVMAIN=${ENABLE_NVMAIN:-1}
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

run() {
  log_info "NVMain Docker entrypoint started."
  
  if [ $ENABLE_NVMAIN == 1 ]; then
    cd /opt/nvmain || exit 0
    
    get_container_number
    
    if [ $COUNT == 0 ] || [ $INSTANCE_INDEX == 1 ]; then
      log_wait "Building artifacts"

      command="scons -j $(nproc) --build-type=fast"
      run_command "$command"
      
      command="scons -j $(nproc) --build-type=prof"
      run_command "$command"
      
      command="scons -j $(nproc) --build-type=debug"
      run_command "$command"
      
      touch /mnt/nvmsim/nvmain-ready
      log_done "Building artifacts"
    fi

    while [ ! -f /mnt/nvmsim/nvmain-ready ]; do 
      log_wait "NVMain main container is busy, wait 10s." 
      sleep 10s
    done

    IFS=', ' read -r -a nvmain_config_files <<< "${NVMAIN_CONFIG_FILES}"
    if [ $COUNT == 0 ]; then
      array_index=0
      nvmain_config_file=${nvmain_config_files[$array_index]}
      command="./nvmain.debug Config/${nvmain_config_file} /mnt/nvmsim/tracefile-instance-1 0 TraceReader=SniperTrace"
    else
      array_index=$(expr $INSTANCE_INDEX - 1)
      nvmain_config_file=${nvmain_config_files[$array_index]}
      command="./nvmain.debug Config/${nvmain_config_file} /mnt/nvmsim/tracefile-instance-${INSTANCE_INDEX} 0 TraceReader=SniperTrace"
    fi
    
    run_command "$command"
    
    log_info "NVMain Docker entrypoint finished." 
    tail -f /dev/null
  else
    log_info "NVMain Docker entrypoint disabled."
    tail -f /dev/null
  fi
}

run
