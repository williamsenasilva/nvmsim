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
git clone git@github.com:SEAL-UCSB/NVmain.git
```

- #### Download do SPEC CPU 2006
Monte a imagem .iso no host. Preencha SPECCPU_PATH do arquivo .env com o caminho da imagem montada no arquivo. 

- #### Criar e configurar arquivo .env
Crie um arquivo .env dentro da raíz do projeto com o comando abaixo
```bash
touch .env
```
Edite as variáveis de ambiente dentro deste arquivo com as pastas do Sniper e PinPlay que foram baixadas

Exemplo:
```conf
SNIPER_PATH=/home/william/Downloads/sniper-7.2
PINPLAY_PATH=/home/william/Downloads/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux
NVMAIN_PATH=/home/william/Downloads/NVmain
SPECCPU_PATH=/run/media/william/SPEC_CPU2006v1.2
SNIPER_TARGET_ARCH=intel64
ENABLE_SNIPER=1
ENABLE_NVMAIN=1
ENABLE_SPECCPU=1
```

- #### Construir imagens Docker
```bash
sh nvmsim.sh build-images
```

- #### Construir ambientes
obs: Arquivos existentes serão removidos e um novo ambiente será recriado do zero 
```bash
sh nvmsim.sh build-envs
```

## Testes

- #### Inicializar containers e rodar trace

```bash
sh nvmsim.sh run
```

- #### Parar containers

```bash
sh nvmsim.sh stop
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)