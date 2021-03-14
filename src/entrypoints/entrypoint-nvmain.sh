#!/bin/bash

ENABLE_NVMAIN=${ENABLE_NVMAIN:-1}
INSTANCE_INDEX=1

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

function run
{
  message_action="[RUNEP]"
  message="NVMain Docker entrypoint started."
  echo "${message_info}${message_action} ${message}"
  
  if [ $ENABLE_NVMAIN == 1 ]; then
    cd /opt/nvmain || exit 0
    
    get_container_number
    
    if [ $INSTANCE_INDEX == 1 ]; then
      scons --build-type=fast
      scons --build-type=prof
      scons --build-type=debug
      touch /mnt/nvmsim/nvmain-ready
    fi

    while [ ! -f /mnt/nvmsim/nvmain-ready ]; do 
      message="NVMain main container is busy, wait 10s."
      echo "${message_info}${message_action} ${message}"
      sleep 10s
    done

    IFS=', ' read -r -a nvmain_config_files <<< "${NVMAIN_CONFIG_FILES}"
    array_index=$(expr $INSTANCE_INDEX - 1)
    nvmain_config_file=${nvmain_config_files[$array_index]}

    command="./nvmain.debug Config/${nvmain_config_file} /mnt/nvmsim/tracefile-instance-${INSTANCE_INDEX} 0 TraceReader=SniperTrace"
    message="command: ${command}"
    echo "${message_info}${message_action} ${message}"
    eval $command
    
    message="NVMain Docker entrypoint finished."
    echo "${message_info}${message_action} ${message}"
  else
    message="NVMain Docker entrypoint disabled."
    echo -e "${message_warn}${message_action} ${message}"
    tail -f /dev/null
  fi
}

run
