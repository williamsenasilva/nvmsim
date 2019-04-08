# NVMSIM

Código fonte mais documentação da aplicação

* #### cópia do projeto 
```bash
git clone git@gitlab.com:williamsenasilva/nvmsim.git
```

* #### construir imagem Docker
```bash
docker build -t nvmsim:latest .
```

* #### inicializar container da imagem criada
```bash
docker run -it --rm --privileged -v $(pwd)/shared:/nvmsim/ nvmsim:latest
```

* #### testar o Sniper
```bash
run-sniper -- /bin/ls
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)
