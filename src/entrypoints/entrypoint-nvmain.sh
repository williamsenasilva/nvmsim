#!/bin/bash

ENABLE_NVMAIN=${ENABLE_NVMAIN:-1}
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
  message="NVMain Docker entrypoint started."
  echo "${message_info}${message_action} ${message}"

  if [ $ENABLE_NVMAIN == 1 ]; then
    cd /opt/nvmain || exit 0
    scons --build-type=fast
    scons --build-type=prof
    scons --build-type=debug
    ./nvmain.debug Config/${NVMSIM_NVM_CONFIG_FILE} /mnt/nvmsim/tracefile 0 TraceReader=SniperTrace
    
    message="NVMain Docker entrypoint finished."
    echo "${message_info}${message_action} ${message}"
    tail -f /dev/null
  else
    message="NVMain Docker entrypoint disabled."
    echo -e "${message_warn}${message_action} ${message}"
    tail -f /dev/null
  fi
}

run
