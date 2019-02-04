FROM debian:9.4

# Update repo list
RUN apt-get update

# Install dependencies
RUN apt-get install -y \
    g++ \ 
    g++-6-multilib \
    git \
    lib32z1-dev \
    libboost-dev \
    libbz2-dev \
    libc6-dev-i386 \
    libsqlite3-dev \
    make \
    python \
    valgrind \
    wget 

# Install helper utilities
RUN apt-get install -y \
    procps \
    vim \
    emacs

RUN mkdir -p /nvmsim/shared

WORKDIR /nvmsim/shared

ENTRYPOINT ["/bin/bash"]