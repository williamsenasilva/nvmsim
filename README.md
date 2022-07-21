# NVMSim

Código fonte mais documentação da aplicação

## Configuração inicial

- #### Download do projeto NVMSim

```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git
```

- #### Download do Sniper
Obtenha a versão mais recente do Sniper (7.3) no <a href="https://snipersim.org/w/Download" target="_blank">site oficial</a>
Será disponibilizado um link para clonar, o repositório. Realize um git clone neste link.
Exemplo
```bash
git clone http://snipersim.org/download/a0123456789b0123456789/git/sniper.git
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

- #### Criar e configurar arquivo .env
Copie o arquivo .env-example para um arquivo .env dentro da raíz do projeto
```bash
cp .env-example .env
```
Edite as variáveis de ambiente dentro deste arquivo de acordo com o seu ambiente

Exemplo:
```conf
SNIPER_PATH=/home/william/Downloads/sniper-7.3
PINPLAY_PATH=/home/william/Downloads/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux
NVMAIN_PATH=/home/william/Downloads/NVmain
SPECCPU_PATH=/home/william/Downloads/SPEC_CPU2006v1.21
ENABLE_SNIPER=1
ENABLE_NVMAIN=1
ENABLE_SPECCPU=0
SNIPER_TARGET_ARCH=intel64
```

- #### Construir imagens Docker
```bash
./run-nvmsim.sh build-images
```

## Testes

- #### Inicializar NVMSim

```bash
./run-nvmsim.sh start
```

- #### Parar NVMSim

```bash
./run-nvmsim.sh stop
```

## Documentação
A documentação está disponível na [Wiki](../../wikis/home)
