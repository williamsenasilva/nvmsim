#!/bin/bash

ENABLE_SPECCPU=${ENABLE_SPECCPU:-1}

source /nvmsim/scripts/util.sh

# todo: Fix this copy. I need to copy because when the shared folder is used as /opt/speccpu the installation fails
copy_speccpu_container_folder_to_host_speccpu_folder() {
  log_wait "Copying /opt/speccpu to host shared folder"
  rm -rf /mnt/speccpu/*
  cp -r /opt/speccpu/* /mnt/speccpu/
  log_done "Copying /opt/speccpu to host shared folder"
}

check_speccpu_requirements() {
  if [ $ENABLE_SPECCPU == 1 ]; then
    if [ ! -f "/mnt/dvd/install.sh" ]; then
      log_errr "File install.sh not found. Check if dvd volume was mounted."
      exit 0
    fi
  else
    log_info "SpecCPU Docker entrypoint disabled."
    tail -f /dev/null
  fi
}

install_speccpu() {
  log_info "Installing speccpu on path /otp/speccpu"
  cd /mnt/dvd || exit 0
  yes | ./install.sh -d /opt/speccpu
  cd /opt/speccpu || exit 0
  log_done "Instalation finished."

  source ./shrc
  cp $SPEC/config/Example-linux64-amd64-gcc43+.cfg $SPEC/config/nvmsim.cfg

  log_info "runspec started."
  runspec --action=run --verbose=99 --size=test --config=nvmsim.cfg all
  log_done "runspec finished."

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
      log_done "${benchmark}"
    else
      log_errr "Binary file ${benchmark_binary} not found."
    fi
  done
}

run() {
  log_info "SpecCPU Docker entrypoint started."

  check_speccpu_requirements
  install_speccpu
  copy_speccpu_container_folder_to_host_speccpu_folder
  
  log_info "SpecCPU Docker entrypoint finished."
  tail -f /dev/null
}

run