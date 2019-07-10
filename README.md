# NVMSIM

Código fonte mais documentação da aplicação

## Configuração inicial

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

## Desenvolvimento

* #### inicializar container da imagem com pasta Sniper compartilhada
```bash
docker run -it --rm --privileged -v $(pwd)/shared:/nvmsim/ -v $(pwd)/shared/sniper/sniper-7.2:/opt/sniper-7.2 nvmsim:latest
```

* #### compilar Sniper
```bash
cd /opt/sniper-7.2 && make
```

* #### testar
```bash
cd /nvmsim/tests && run-sniper -c gainestown -c memory.cfg -d /nvmsim/tests -- /bin/true
cat sim.out
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)
