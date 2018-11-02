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
docker build .
```

* pegar o image_id da imagem criada
```bash
docker images
```
exemplo
```
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
<none>              <none>              4b90f69ca885        2 minutes ago      492MB
```
Neste caso o image_id é **4b90f69ca885**

* inicializar container da imagem criada
```bash
docker run -v caminho_do_clone/shared/:/root/shared -ti image_id /bin/bash
```

* executar script de instalação do sniper no container
```bash
cd /root/shared
chmod +x install.sh 
./install.sh
```

## Documentação

A documentação está disponível na [Wiki](https://gitlab.com/williamsenasilva/nvmsim/wikis/home)