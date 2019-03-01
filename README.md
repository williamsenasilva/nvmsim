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

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)