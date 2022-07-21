#!/bin/bash

SCRIPT_ARGS=("$@")

path=$(pwd)
shared_path=${path}/shared
vendor_path=${path}/artifacts
scripts_path=${path}/scripts
env_file_path=$path'/.env'

source ${scripts_path}/util.sh
source ${scripts_path}/help.sh

build_images() {
  command="docker-compose build ${SCRIPT_ARGS[1]} ${SCRIPT_ARGS[2]}"
  run_command "$command"

: <<'END'
  if [ ! -d "${SPECCPU_PATH}" ]; then
    log_errr "Path \"${SPECCPU_PATH}\" not found found. Check if dvd volume was mounted."
    exit 0
  fi

  log_info "Copying SPECCPU midia to temp path to be used in Docker context... "
  if [ -d "${path}/temp/speccpu-midia" ]; then
    rm -rf ${path}/temp/speccpu-midia
  fi
  mkdir -p ${path}/temp
  cp -r "${SPECCPU_PATH}" ${path}/temp/speccpu-midia
END

  #docker-compose build speccpu
}

copy_simulators_files() {
  if [ $SNIPER_PATH ] && [ $PINPLAY_PATH ] && [ $NVMAIN_PATH ]; then
    if ! test -f "${path}/shared/sniper/run-sniper"; then
      log_wait "Copying artifacts"
      command="cp -r ${SNIPER_PATH} ${path}/shared/sniper"
      run_command "$command"
      log_done "Copying artifacts ($command)"
    fi

    if ! test -f "${path}/shared/pinplay/pin"; then
      log_wait "Copying artifacts"
      command="cp -r ${PINPLAY_PATH} ${path}/shared/pinplay"
      run_command "$command"
      log_done "Copying artifacts ($command)"
    fi

    if ! test -f "${path}/shared/nvmain/nvmain.debug"; then
      log_wait "Copying artifacts"
      command="cp -r ${NVMAIN_PATH} ${path}/shared/nvmain"
      run_command "$command"
      log_done "Copying artifacts ($command)"
    fi
  else
    log_errr "You must define SNIPER_PATH, PINPLAY_PATH, and NVMAIN_PATH in your .env file."
    exit 0
  fi
}

copy_source_files() {
  if [ $ENABLE_SNIPER == 1 ] && [ $ENABLE_NVMAIN == 1 ]; then
    log_wait "Copying source files"
    
    # NVMSim files
    cp -r ${path}/src/helper/c_programs/cache/ ${path}/shared/nvmsim/src/
    
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
    cp ${path}/src/nvmain/CoinMigrator.cpp ${path}/shared/nvmain/Utils/CoinMigrator/
    cp ${path}/src/nvmain/SConscript ${path}/shared/nvmain/
    cp ${path}/src/nvmain/2D_DRAM_example.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/3D_DRAM_example.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/PCM_MLC_example.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/RRAM_ISSCC_2012_4GB.config ${path}/shared/nvmain/Config/
    cp ${path}/src/nvmain/STTRAM_Everspin_4GB.config ${path}/shared/nvmain/Config/

    log_done "Copying source files"
  fi
}

update_gid_in_dot_env() {
  text_to_find="GROUP_ID"
  text_to_replace="GROUP_ID="$(id -g)
  file=$path'/.env'
  if grep -q "$text_to_find" $path'/.env'; then
    line_number=$(cat -n $path/.env | grep "$text_to_find")
    line_number=$(echo $line_number | cut -d ' ' -f1 | tr -d ' ')
    sed -i $line_number's/.*/'$text_to_replace'/' $file
    log_info "$text_to_replace"
  else
    echo $text_to_replace >> $path'/.env'
    log_info "$text_to_replace"
  fi
}

update_uid_in_dot_env() {
  text_to_find="USER_ID"
  text_to_replace="USER_ID="$(id -u)
  file=$path'/.env'
  if grep -q "$text_to_find" $path'/.env'; then
    line_number=$(cat -n $path/.env | grep "$text_to_find")
    line_number=$(echo $line_number | cut -d ' ' -f1 | tr -d ' ')
    sed -i $line_number's/.*/'$text_to_replace'/' $file
    log_info "$text_to_replace"
  else
    echo $text_to_replace >> $path'/.env'
    log_info "$text_to_replace"
  fi
}

get_environment_values() {
  sed -i 's/\r$//' ${path}/.env
  source ${path}/.env
}

remove_gem5_dependency() {
  sed -i -e 's/from gem5_scons import Transform/#from gem5_scons import Transform/g' ${path}/shared/nvmain/SConscript
}

restart_services() {
  command="docker-compose restart -t 1 $1 $2 $3"
  run_command "$command"
}

show_logs() {
  command="docker-compose logs -f $1 $2 $3"
  run_command "$command"
}

start_services() {
  rm -rf ${path}/shared/nvmsim/*

  copy_simulators_files
  remove_gem5_dependency
  copy_source_files
  
  IFS=', ' read -r -a config_files <<< "${NVMAIN_CONFIG_FILES}"
  instances=${#config_files[@]}
  command="docker-compose up -d $1 $2 $3 --scale sniper=$instances --scale nvmain=$instances"
  run_command "$command"
}

stop_services() {
  log_info ""
  command="docker-compose down $1 $2 $3"
  run_command "$command"
}

_main() {
  update_gid_in_dot_env
  update_uid_in_dot_env
  get_environment_values

  # build, start, restart, stop... até 3 serviços (sniper, nvmain, speccpu)
  case ${SCRIPT_ARGS[0]} in
    build-images)
      stop_services "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      build_images "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      ;;
    help)
      _help
      ;;
    logs)
      show_logs "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      ;;
    restart)
      restart_services "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      show_logs "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      ;;
    start)
      stop_services "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      start_services "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      show_logs "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      ;;
    stop)
      stop_services "${SCRIPT_ARGS[1]}" "${SCRIPT_ARGS[2]}" "${SCRIPT_ARGS[3]}"
      ;;
    *)
      log_errr "The command \"${SCRIPT_ARGS}\" is invalid. Please, use ${BLUE}${0} help${CLEAR} to get more information"
      exit 0
      ;;
  esac
}
 
_main
