#!/bin/bash

nvmsim_script_action=$1
docker_compose_optional_arg=$2

path=$(pwd)
cd $path

# get .env values
sed -i 's/\r$//' ${path}/.env
source ${path}/.env

SNIPER_PATH="${SNIPER_PATH:-}"
PINPLAY_PATH="${PINPLAY_PATH:-}"
NVMAIN_PATH="${NVMAIN_PATH:-}"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW="\033[0;33m"
NOCOLOR='\033[0m'

message_info="[NVMSIM][INFO ]"
message_warn="[NVMSIM][${YELLOW}WARN${NOCOLOR} ]"
message_erro="[NVMSIM][${RED}ERROR${NOCOLOR}]"
message_done="[NVMSIM][${GREEN}DONE${NOCOLOR} ]"

nvmsim_script_action=${nvmsim_script_action:-run}

function run
{
  message_action="[START]"
  echo -e "${NOCOLOR}${message_info}${message_action} NVMSIM main script started."

  if [ $nvmsim_script_action == build-images ]; then
    docker-compose down
    docker-compose build ${docker_compose_optional_arg}

    message_action="[CHECK]"
    if [ ! -d "${SPECCPU_PATH}" ]; then
      message="Path \"${SPECCPU_PATH}\" not found found. Check if dvd volume was mounted."
      echo -e "${message_erro}${message_action} ${message}"
      exit 0
    fi

    message_action="[COPY ]"
    message="Copying SPECCPU midia to temp path to be used in Docker context... "
    echo -n "${message_info}${message_action} ${message}"
    if [ -d "${path}/temp/speccpu-midia" ]; then
      rm -rf ${path}/temp/speccpu-midia
    fi
    mkdir -p ${path}/temp
    cp -r "${SPECCPU_PATH}" ${path}/temp/speccpu-midia
    echo "ok"

    docker-compose build speccpu
  elif [ $nvmsim_script_action == run ]; then
    docker-compose down
    remove_gem5_dependency
    copy_source_files
    
    rm -rf ${path}/shared/nvmsim/logs/
    
    if test -f "${path}/shared/nvmsim/nvmain-ready"; then
      rm -f ${path}/shared/nvmsim/nvmain-ready
    fi

    if test -f "${path}/shared/nvmsim/sniper-ready"; then
      rm -f ${path}/shared/nvmsim/sniper-ready
    fi

    IFS=', ' read -r -a config_files <<< "${NVMAIN_CONFIG_FILES}"
    instances=${#config_files[@]}
    command="docker-compose up -d --scale sniper=$instances --scale nvmain=$instances"
    message="command: ${command}"
    echo "${message_info}${message_action} ${message}"
    eval $command
    
    docker-compose logs -f
  elif [ $nvmsim_script_action == stop ]; then
    docker-compose down
  else
    message_action="[START]"
    message="Argument \"${nvmsim_script_action}\" not accepted. Usage: nvmsim.sh [run, stop, build-images, build-envs]"
    echo -e "${message_erro}${message_action} ${message}"
    exit 0
  fi
}

function remove_gem5_dependency
{
  sed -i -e 's/from gem5_scons import Transform/#from gem5_scons import Transform/g' ${path}/shared/nvmain/SConscript
}

function copy_third_source_files
{
  message_action="[COPY ]"
  if [ $SNIPER_PATH ] && [ $PINPLAY_PATH ] && [ $NVMAIN_PATH ]; then
    message="Copying artifacts... "
    echo -n "${message_info}${message_action} ${message}"
    rm -rf ${path}/shared/sniper/
    rm -rf ${path}/shared/pinplay/
    rm -rf ${path}/shared/nvmain/
    cp -r ${SNIPER_PATH} ${path}/shared/sniper
    cp -r ${PINPLAY_PATH} ${path}/shared/pinplay
    cp -r ${NVMAIN_PATH} ${path}/shared/nvmain
    echo "ok"
  else
    message="${message} You must define SNIPER_PATH, PINPLAY_PATH, and NVMAIN_PATH in your .env file."
    echo -e "${message_erro}${message_action} ${message}"
    exit 0
  fi
}

function copy_source_files
{
  if [ $ENABLE_SNIPER == 1 ] && [ $ENABLE_NVMAIN == 1 ]; then
    message_action="[COPY ]"
    message="Copying source files... "
    echo -n "${message_info}${message_action} ${message}"
    # Sniper files
    cp ${path}/src/sniper/core.cc ${path}/shared/sniper/common/core/
    cp ${path}/src/sniper/cache_cntlr.cc ${path}/shared/sniper/common/core/memory_subsystem/parametric_dram_directory_msi/
    cp ${path}/src/sniper/dram_cntlr.cc ${path}/shared/sniper/common/core/memory_subsystem/pr_l1_pr_l2_dram_directory_msi/
    cp ${path}/src/sniper/dram_cntlr.h ${path}/shared/sniper/common/core/memory_subsystem/pr_l1_pr_l2_dram_directory_msi/
    cp ${path}/src/sniper/memory_manager.cc ${path}/shared/sniper/common/core/memory_subsystem/parametric_dram_directory_msi/
    cp ${path}/src/sniper/dram_cache.cc ${path}/shared/sniper/common/core/memory_subsystem/dram/
    cp ${path}/src/sniper/dram_perf_model.cc ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model.h ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_normal.cc ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_normal.h ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_constant.cc ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_constant.h ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_readwrite.cc ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_readwrite.h ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_nvm.cc ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_perf_model_nvm.h ${path}/shared/sniper/common/performance_model/
    cp ${path}/src/sniper/dram_cntlr_interface.cc ${path}/shared/sniper/common/core/memory_subsystem/dram/
    cp ${path}/src/sniper/nvmsim-ram.cfg ${path}/shared/sniper/config/
    cp ${path}/src/sniper/nvmsim-nvm.cfg ${path}/shared/sniper/config/
    # c program
    # gcc -O0 ${path}/src/helper/c_programs/single-line-cache.c -o ${path}/shared/sniper/nvmsim
    gcc -O0 ${path}/src/helper/c_programs/multiple-line-cache.c -o ${path}/shared/sniper/nvmsim
    # NVMain files
    mkdir -p ${path}/shared/nvmain/traceReader/SniperTrace
    cp ${path}/src/nvmain/GenericTraceReader.h ${path}/shared/nvmain/traceReader/
    cp ${path}/src/nvmain/TraceReaderFactory.cpp ${path}/shared/nvmain/traceReader/
    cp ${path}/src/nvmain/SniperTraceReader.cpp ${path}/shared/nvmain/traceReader/SniperTrace/
    cp ${path}/src/nvmain/SniperTraceReader.h ${path}/shared/nvmain/traceReader/SniperTrace/
    cp ${path}/src/nvmain/FCFS.h ${path}/shared/nvmain/MemControl/FCFS/
    cp ${path}/src/nvmain/FCFS.cpp ${path}/shared/nvmain/MemControl/FCFS/
    cp ${path}/src/nvmain/FRFCFS.h ${path}/shared/nvmain/MemControl/FRFCFS/
    cp ${path}/src/nvmain/FRFCFS.cpp ${path}/shared/nvmain/MemControl/FRFCFS/
    cp ${path}/src/nvmain/FRFCFS-WQF.h ${path}/shared/nvmain/MemControl/FRFCFS-WQF/
    cp ${path}/src/nvmain/FRFCFS-WQF.cpp ${path}/shared/nvmain/MemControl/FRFCFS-WQF/
    cp ${path}/src/nvmain/nvmain.h ${path}/shared/nvmain/NVM/
    cp ${path}/src/nvmain/nvmain.cpp ${path}/shared/nvmain/NVM/
    cp ${path}/src/nvmain/MemoryController.h ${path}/shared/nvmain/src/
    cp ${path}/src/nvmain/MemoryController.cpp ${path}/shared/nvmain/src/
    cp ${path}/src/nvmain/traceMain.cpp ${path}/shared/nvmain/traceSim/
    cp ${path}/src/nvmain/SConscript ${path}/shared/nvmain/
    cp ${path}/src/nvmain/2D_DRAM_example.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/3D_DRAM_example.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/PCM_MLC_example.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/Hybrid_example.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/tracefile.nvt ${path}/shared/nvmsim/

    echo "ok"
  fi
}
 
run
