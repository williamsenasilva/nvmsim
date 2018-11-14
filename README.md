# NVMSIM

Código fonte mais documentação da aplicação

## Começando
* clone do projeto
```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git

```
* construir imagem Docker
```bash
cd caminho_do_clone
docker build . -t nvmsim:0.1
```

* inicializar container da imagem criada
```bash
docker run -v caminho_do_clone/shared/:/root/shared --privileged -ti nvmsim:0.1 /bin/bash  -c 'cd /root/shared; exec "${SHELL:-sh}"'
```

* executar script de instalação do sniper no container
```bash
chmod +x install.sh 
./install.sh
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)