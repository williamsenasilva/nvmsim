#!/bin/bash

tabs 3

PIPE="${RED}|${CLEAR}"

_help () {
  echo ""
  echo -e "${YELLOW}NAME${CLEAR}"
  echo -e "   ${0} - script para exsecução do ${BOLD}NVMSim${CLEAR}\n"
  echo -e "${YELLOW}SYNOPSIS${CLEAR}"
  echo -e "   ${0} COMMAND [ run ${PIPE} stop ${RED}...${CLEAR} ]\n"
  echo -e "   COMMAND ${RED}:=${CLEAR} { run ${PIPE} stop ${PIPE} logs ${PIPE} build-images ${PIPE} }    ${BLUE}DEFAULT${CLEAR}: run\n"
  echo -e "${YELLOW}DESCRIPTION${CLEAR}"
  echo -e "   Este é o principal script para executar todas as rotinas do ${BOLD}NVMSim${CLEAR}."
  echo -e "   Ele é responsável por executar várias rotinas bash/Docker/Docker-Compose através de alguns comandos."
  echo -e "   O ${BOLD}NVMSim${CLEAR} utiliza estes comandos mais variáveis de ambiente definidas no arquivo .env"
  echo -e "   Veja o README abaixo para mais informações."
  echo -e "   https://gitlab.com/williamsenasilva/nvmsim/-/blob/master/README.md\n"
    
  echo -e "${YELLOW}COMMANDS${CLEAR}"

  echo -e "   ${BOLD}start${CLEAR}"
  echo -e "      ${BLUE}# ${CLEAR}${0} start <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>   ${BLUE}DEFAULT${CLEAR}: sniper nvmain"
  echo -e "      ${BLUE}?${CLEAR} Inicializa os containers\n"
    
  echo -e "   ${BOLD}restart${CLEAR}"
  echo -e "      ${BLUE}# ${CLEAR}${0} restart <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>   ${BLUE}DEFAULT${CLEAR}: sniper nvmain"
  echo -e "      ${BLUE}?${CLEAR} Reinicializa os containers\n"

  echo -e "   ${BOLD}stop${CLEAR}"
  echo -e "      ${BLUE}# ${CLEAR}${0} stop <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>   ${BLUE}DEFAULT${CLEAR}: sniper nvmain"
  echo -e "      ${BLUE}?${CLEAR} Encerra os containers\n"

  echo -e "   ${BOLD}logs${CLEAR}"
  echo -e "      ${BLUE}# ${CLEAR}${0} logs <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>    ${BLUE}DEFAULT${CLEAR}: all"
  echo -e "      ${BLUE}?${CLEAR} Exibe os logs dos containers em execução\n"

  echo -e "   ${BOLD}build-images${CLEAR}"
  echo -e "      ${BLUE}# ${CLEAR}${0} build-images <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>    ${BLUE}DEFAULT${CLEAR}: all"
  echo -e "      ${BLUE}?${CLEAR} Gera as imagens necessárias para execução dos testes em containers.\n"

  echo -e "${YELLOW}EXAMPLES${CLEAR}"
  echo -e "   ${BOLD}${0} start sniper nvmain${CLEAR}"
  echo -e "   ${BOLD}${0} stop${CLEAR}"
  echo -e "   ${BOLD}${0} build-images${CLEAR}"
  echo -e "   ${BOLD}${0} logs${CLEAR}"
  echo -e "   ${BOLD}${0} logs sniper nvmain${CLEAR}\n"
}