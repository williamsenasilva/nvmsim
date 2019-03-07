# NVMSIM

Código fonte mais documentação da aplicação

## Começando

* **clone do projeto e configuração de variável de ambiente**
```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git; cd nvmsim; echo export NVMSIMPATH=$(pwd) >> ~/.bashrc; source ~/.bashrc
```

* **construir imagem Docker**
```bash
docker build . -t nvmsim:1.0.0
```

* **inicializar container da imagem criada**
```bash
docker run -it --rm --privileged -v $NVMSIMPATH/shared:/nvmsim/ nvmsim:1.0.0
```

## Create images
```bash
docker build . -t nvmsim/ubuntu-18.04-sniper-7.1-pin-3.7:1.0.0 -f Dockerfile-ubuntu-18.04-sniper-7.1-pin-3.7
docker build . -t nvmsim/ubuntu-18.04-sniper-7.1-pinplay-3.5:1.0.0 -f Dockerfile-ubuntu-18.04-sniper-7.1-pinplay-3.5
docker build . -t nvmsim/ubuntu-18.04-sniper-7.2-pin-3.7:1.0.0 -f Dockerfile-ubuntu-18.04-sniper-7.2-pin-3.7
docker build . -t nvmsim/ubuntu-18.04-sniper-7.2-pinplay-3.5:1.0.0 -f Dockerfile-ubuntu-18.04-sniper-7.2-pinplay-3.5

docker build . -t nvmsim/debian-9.4-sniper-7.1-pin-3.7:1.0.0 -f Dockerfile-debian-9.4-sniper-7.1-pin-3.7
docker build . -t nvmsim/debian-9.4-sniper-7.1-pinplay-3.5:1.0.0 -f Dockerfile-debian-9.4-sniper-7.1-pinplay-3.5
docker build . -t nvmsim/debian-9.4-sniper-7.2-pin-3.7:1.0.0 -f Dockerfile-debian-9.4-sniper-7.2-pin-3.7
docker build . -t nvmsim/debian-9.4-sniper-7.2-pinplay-3.5:1.0.0 -f Dockerfile-debian-9.4-sniper-7.2-pinplay-3.5
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)