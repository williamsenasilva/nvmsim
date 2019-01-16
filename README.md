# NVMSIM

Código fonte mais documentação da aplicação

## Primeiros passos

* clone do projeto e configuração de variável de ambiente
```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git
cd nvmsim
echo export NVMSIMPATH=$(pwd) >> ~/.bashrc
source ~/.bashrc
```

* construir imagem Docker
```bash
docker build . -t nvmsim:0.1
```

* inicializar container da imagem criada
```bash
docker run --rm --privileged -ti -v $NVMSIMPATH/shared/:/root/shared nvmsim:0.1 
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)