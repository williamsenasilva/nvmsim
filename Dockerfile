FROM debian:9.5
# Update repo list
RUN apt-get update
# Install dependencies
RUN apt-get install -y \
    libc6-dev-i386 \
    lib32z1-dev \
    g++-6-multilib \
    make \
    wget \
    python \
    git \
    libbz2-dev \
    g++ \
    libboost-dev \
    libsqlite3-dev
# Install helper utilities
RUN apt-get install -y \
    procps \
    vim
# TODO: Install benchmarks dependencies
#RUN apt-get install -y \
#    gfortran \
#    m4 \
#    xsltproc \
#    pkg-config \
#    gettext \
#    libx11-dev \
#    libxext-dev \
#    libxt-dev \
#    libxmu-dev \
#    libxi-dev