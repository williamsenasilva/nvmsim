# NVMSIM

Código fonte mais documentação da aplicação

## Começando

* **clone do projeto e configuração de variável de ambiente**
```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git && cd nvmsim && echo export NVMSIMPATH=$(pwd) >> ~/.bashrc && source ~/.bashrc
```

* **construir imagem Docker**
```bash
docker build . -t nvmsim:1.0.0
```

* **inicializar container da imagem criada**
```bash
docker run -it --rm --privileged -v $NVMSIMPATH/shared/:/nvmsim/shared nvmsim:1.0.0
```

* **executar script de instalação do sniper no container**
```bash
chmod +x shared/install_sniper.sh 
. shared/install_sniper.sh
```

* **salvar nova imagem com sniper instalado**

tecle ctrl + p + q 

Depois execute:
```bash
docker ps
```

Copie o CONTAINER ID e cole no comando abaixo no lugar de container_id
```bash
docker commit container_id nvmsim:1.0.1
```

exemplo
```bash
docker commit 4eaf629a8cf9 nvmsim:1.0.1
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)