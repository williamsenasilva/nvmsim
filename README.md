# NVMSIM

Código fonte mais documentação da aplicação

## Começando
* clone do projeto e configuração de variável de ambiente
```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git
cd nvmsim
{ echo export NVMSIMPATH=; pwd; } | tr "\n" " " | sed 's/= /=/g'  >> ~/.bashrc
source ~/.bashrc
```
* construir imagem Docker
```bash
docker build . -t nvmsim:0.1
```

* inicializar container da imagem criada
```bash
docker run --privileged -it -v $NVMSIMPATH/shared/:/root/shared nvmsim:0.1 
```

* executar script de instalação do sniper no container
```bash
chmod +x install.sh 
./install.sh
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)