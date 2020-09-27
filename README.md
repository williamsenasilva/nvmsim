# NVMSIM

Código fonte mais documentação da aplicação

## Configuração inicial

- #### Download do projeto NVMSIM

```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git
```

- #### Download do Sniper 
Obtenha a versão mais recente do Sniper (7.3) no <a href="https://snipersim.org/w/Download" target="_blank">site oficial</a>  
Será disponibilizado um link para clonar, o repositório. Realize um git clone neste link.
Exemplo
```bash
git clone http://snipersim.org/download/b98cd4a4c864ce6d/git/sniper.git
```

- #### Download do PinPlay 
Obtenha a versão mais recente do PinPlay (3.7) no <a href="https://software.intel.com/content/www/us/en/develop/articles/program-recordreplay-toolkit.html" target="_blank">site oficial</a>  
Descompacte o arquivo tar.gz baixado

Exemplo
```bash
tar -xf pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux.tar.gz
```

- #### Download do NVMain 
Obtenha a versão mais recente do NVMain por download no <a href="https://bitbucket.org/mrp5060/nvmain" target="_blank">bitbucket</a> 
Ou via hg clone do projeto.
Exemplo
```bash
hg clone https://bitbucket.org/mrp5060/nvmain
```

- #### Criar e configurar arquivo .env
Crie um arquivo .env dentro da raíz do projeto com o comando abaixo
```bash
touch .env
```
Edite as variáveis de ambiente dentro deste arquivo com as pastas do Sniper e PinPlay que foram baixadas

Exemplo:
```conf
SNIPER_PATH=/home/william/Downloads/sniper
PINPLAY_PATH=/home/william/Downloads/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux
NVMAIN_PATH=/home/william/Downloads/nvmain
```

- #### Construir imagens Docker
```bash
sh nvmsim.sh build
```

## Testes

- #### Inicializar containers e rodar trace

```bash
sh nvmsim.sh
```

- #### Parar containers

```bash
sh nvmsim.sh stop
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)