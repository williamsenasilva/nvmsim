#!/bin/bash

tabs 3

PIPE="${RED}|${CLEAR}"

function _help 
{
    echo ""
    echo -e "${YELLOW}NAME${CLEAR}"
    echo -e "\t${0} - script para exsecução do ${BOLD}NVMSIM${CLEAR}\n"
    echo -e "${YELLOW}SYNOPSIS${CLEAR}"
    echo -e "\t${0} COMMAND [ run ${PIPE} stop ${RED}...${CLEAR} ]\n"
    echo -e "\tCOMMAND ${RED}:=${CLEAR} { run ${PIPE} stop ${PIPE} logs ${PIPE} build-images ${PIPE} } \t${BLUE}DEFAULT${CLEAR}: run\n"
    echo -e "${YELLOW}DESCRIPTION${CLEAR}"
    echo -e "\tEste é o principal script para executar todas as rotinas do ${BOLD}NVMSIM${CLEAR}."
    echo -e "\tEle é responsável por executar várias rotinas bash/Docker/Docker-Compose através de alguns comandos."
    echo -e "\tO ${BOLD}NVMSIM${CLEAR} utiliza estes comandos mais variáveis de ambiente definidas no arquivo .env"
    echo -e "\tVeja o README abaixo para mais informações."
    echo -e "\thttps://gitlab.com/williamsenasilva/nvmsim/-/blob/master/README.md\n"
    
    echo -e "${YELLOW}COMMANDS${CLEAR}"

    echo -e "\t${BOLD}run${CLEAR}"
    echo -e "\t\t${BLUE}# ${CLEAR}${0} run <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>\t${BLUE}DEFAULT${CLEAR}: sniper nvmain"
    echo -e "\t\t${BLUE}?${CLEAR} Inicializa os containers\n"
    
    echo -e "\t${BOLD}restart${CLEAR}"
    echo -e "\t\t${BLUE}# ${CLEAR}${0} restart <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>\t${BLUE}DEFAULT${CLEAR}: sniper nvmain"
    echo -e "\t\t${BLUE}?${CLEAR} Reinicializa os containers\n"

    echo -e "\t${BOLD}stop${CLEAR}"
    echo -e "\t\t${BLUE}# ${CLEAR}${0} stop <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu>\t${BLUE}DEFAULT${CLEAR}: sniper nvmain"
    echo -e "\t\t${BLUE}?${CLEAR} Encerra os containers\n"

    echo -e "\t${BOLD}logs${CLEAR}"
    echo -e "\t\t${BLUE}# ${CLEAR}${0} logs <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu> \t${BLUE}DEFAULT${CLEAR}: all"
    echo -e "\t\t${BLUE}?${CLEAR} Exibe os logs dos containers em execução\n"

    echo -e "\t${BOLD}build-images${CLEAR}"
    echo -e "\t\t${BLUE}# ${CLEAR}${0} build-images <all${PIPE}sniper${PIPE}nvmain${PIPE}speccpu> \t${BLUE}DEFAULT${CLEAR}: all"
    echo -e "\t\t${BLUE}?${CLEAR} Gera as imagens necessárias para execução dos testes em containers.\n"

    echo -e "${YELLOW}EXAMPLES${CLEAR}"
    echo -e "\t${BOLD}${0} run sniper nvmain${CLEAR}"
    echo -e "\t${BOLD}${0} stop${CLEAR}"
    echo -e "\t${BOLD}${0} build-images${CLEAR}"
    echo -e "\t${BOLD}${0} logs${CLEAR}"
    echo -e "\t${BOLD}${0} logs sniper nvmain${CLEAR}\n"
}