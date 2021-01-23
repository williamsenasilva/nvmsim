#!/bin/bash

nvmsim_script_action=$1
docker_compose_optional_arg=$2

path=$(pwd)
cd $path
source ${path}/.env

sniper_path="${SNIPER_PATH:-}"
pinplay_path="${PINPLAY_PATH:-}"
nvmain_path="${NVMAIN_PATH:-}"
sniper_arch="${SNIPER_TARGET_ARCH:-intel64}"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW="\033[0;33m"
NOCOLOR='\033[0m'

echo -e "${NOCOLOR}[NVMSIM][INFO ] NVMSIM started."

function copy_third_source_files
{
  if [ $sniper_path ] && [ $pinplay_path ] && [ $nvmain_path ]; then
    echo "[NVMSIM][INFO ] Copying artifacts... "
    rm -rf ${path}/shared/sniper
    rm -rf ${path}/shared/pinplay
    rm -rf ${path}/shared/nvmain
    cp -r ${sniper_path} ${path}/shared/sniper
    cp -r ${pinplay_path} ${path}/shared/pinplay
    cp -r ${nvmain_path} ${path}/shared/nvmain
    echo -e "[NVMSIM][INFO ] Copying artifacts... ${GREEN}OK${NOCOLOR}"
  else
    echo -e "[NVMSIM][${RED}ERROR${NOCOLOR}] You must define SNIPER_PATH, PINPLAY_PATH, and NVMAIN_PATH in your .env file."
    exit 1
  fi
}

function copy_source_files
{
  echo "[NVMSIM][INFO ] Copying source files... "
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
  cp ${path}/src/sniper/nvmsim.cfg ${path}/shared/sniper/config/
  # c program
  # gcc -O0 ${path}/src/helper/c_programs/single-line-cache.c -o ${path}/shared/sniper/nvmsim
  gcc -O0 ${path}/src/helper/c_programs/multiple-line-cache.c -o ${path}/shared/sniper/nvmsim
  # NVMain files
  mkdir -p ${path}/shared/nvmain/traceReader/SniperTrace
  cp ${path}/src/nvmain/GenericTraceReader.h ${path}/shared/nvmain/traceReader
  cp ${path}/src/nvmain/TraceReaderFactory.cpp ${path}/shared/nvmain/traceReader
  cp ${path}/src/nvmain/SniperTraceReader.cpp ${path}/shared/nvmain/traceReader/SniperTrace
  cp ${path}/src/nvmain/SniperTraceReader.h ${path}/shared/nvmain/traceReader/SniperTrace
  cp ${path}/src/nvmain/FCFS.h ${path}/shared/nvmain/MemControl/FCFS
  cp ${path}/src/nvmain/FCFS.cpp ${path}/shared/nvmain/MemControl/FCFS
  cp ${path}/src/nvmain/FRFCFS.h ${path}/shared/nvmain/MemControl/FRFCFS
  cp ${path}/src/nvmain/FRFCFS.cpp ${path}/shared/nvmain/MemControl/FRFCFS
  cp ${path}/src/nvmain/FRFCFS-WQF.h ${path}/shared/nvmain/MemControl/FRFCFS-WQF
  cp ${path}/src/nvmain/FRFCFS-WQF.cpp ${path}/shared/nvmain/MemControl/FRFCFS-WQF
  cp ${path}/src/nvmain/nvmain.h ${path}/shared/nvmain/NVM
  cp ${path}/src/nvmain/nvmain.cpp ${path}/shared/nvmain/NVM
  cp ${path}/src/nvmain/MemoryController.h ${path}/shared/nvmain/src
  cp ${path}/src/nvmain/MemoryController.cpp ${path}/shared/nvmain/src
  cp ${path}/src/nvmain/traceMain.cpp ${path}/shared/nvmain/traceSim
  cp ${path}/src/nvmain/SConscript ${path}/shared/nvmain
  cp ${path}/src/nvmain/tracefile.nvt ${path}/shared/nvmsim
  echo -e "[NVMSIM][INFO ] Copying source files... ${GREEN}OK${NOCOLOR}"
}

nvmsim_script_action=${nvmsim_script_action:-run}

if [ $nvmsim_script_action == build-images ]; then
  docker-compose down
  docker-compose build ${docker_compose_optional_arg}
elif [ $nvmsim_script_action == build-envs ]; then
  docker-compose down
  copy_third_source_files
  copy_source_files
  # remove gem5 dependency
  sed -i -e 's/from gem5_scons import Transform/#from gem5_scons import Transform/g' ${path}/shared/nvmain/SConscript
  docker-compose up -d && docker-compose logs -f
elif [ $nvmsim_script_action == run ]; then
  docker-compose down
  copy_source_files
  rm -rf ${path}/shared/nvmsim/logs/
  docker-compose up -d && docker-compose logs -f
elif [ $nvmsim_script_action == stop ]; then
  docker-compose down
else
  echo -e "[NVMSIM][${RED}ERROR${NOCOLOR}] Argument \"${nvmsim_script_action}\" not accepted."
  echo "[NVMSIM][INFO ] Usage: nvmsim.sh [run, stop, build-images, build-envs]"
  exit 1
fi
