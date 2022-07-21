#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW="\033[0;33m"
CLEAR='\033[0m'
BLUE='\e[94m'
BOLD='\e[37m'
PIPE="${RED}|${CLEAR}"

log_done() {
  local message=$1
  printf "[NVMSIM][${GREEN}DONE${CLEAR} ][$(basename "$0")][${FUNCNAME[1]}()] $message\n"
}

log_errr() {
  local message=$1
  printf "[NVMSIM][${RED}ERROR${CLEAR}][$(basename "$0")][${FUNCNAME[1]}()] $message\n"
}

log_info() {
  local message=$1
  printf "[NVMSIM][INFO ][$(basename "$0")][${FUNCNAME[1]}()] $message\n"
}

log_wait() {
  local message=$1
  printf "[NVMSIM][${BLUE}WAIT${CLEAR} ][$(basename "$0")][${FUNCNAME[1]}()] $message\n"
}

log_warn() {
  local message=$1
  printf "[NVMSIM][${YELLOW}WARN${CLEAR} ][$(basename "$0")][${FUNCNAME[1]}()] $message\n"
}

log_debg() {
  local message=$1
  printf "[NVMSIM][DEBUG][$(basename "$0")][${FUNCNAME[1]}()] $message\n"
}

run_command() {
  log_info "$1"
  eval $1
}
