# NVMSIM

Código fonte mais documentação da aplicação

## Configuração inicial

-   #### cópia do projeto

```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git
```

-   #### construir imagens Docker

```bash
docker build -t nvmsim/sniper:latest -f src/sniper/Dockerfile .
docker build -t nvmsim/nvmain:latest -f src/nvmain/Dockerfile .
```

## Testes

-   #### inicializar containers

```bash
. run-nvmsim.sh
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)
