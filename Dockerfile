FROM ubuntu:18.04

# Add i386 support for support for Pin
RUN dpkg --add-architecture i386
RUN apt-get update && apt-get install -y \
    python \
    screen \
    tmux \
    binutils \
    libc6:i386 \
    libncurses5:i386 \
    libstdc++6:i386 \
    && rm -rf /var/lib/apt/lists/*

# For building Sniper
RUN apt-get update && apt-get install -y \
    automake \
    build-essential \
    curl \
    wget \
    libboost-dev \
    libsqlite3-dev \
    zlib1g-dev \
    libbz2-dev \
    g++-6 \
    && rm -rf /var/lib/apt/lists/*

# For building RISC-V Tools
RUN apt-get update && apt-get install -y \
    autoconf \
    automake \
    autotools-dev \
    bc \
    bison \
    curl \
    device-tree-compiler \
    flex \
    gawk \
    gperf \
    libexpat1-dev \
    libgmp-dev \
    libmpc-dev \
    libmpfr-dev \
    libtool \
    libusb-1.0-0-dev \
    patchutils \
    pkg-config \
    texinfo \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# For building SPEC CPU2006
RUN apt-get update && apt-get install -y \
    gfortran

# Helper utilities
RUN apt-get update && apt-get install -y \
    gdb \
    git \
    procps \
    vim \
    emacs \
    && rm -rf /var/lib/apt/lists/*

ADD vendor/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux.tar.gz /opt
ENV PIN_HOME=/opt/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux

ADD vendor/sniper-7.2.tgz /opt
ENV SNIPER_HOME=/opt/sniper-7.2

RUN cd /opt/sniper-7.2/; make

RUN echo "alias run-sniper='/opt/sniper-7.2/run-sniper'" >> ~/.bashrc
RUN echo "alias test-sniper='cd /opt/sniper-7.2/test/fft; make run'" >> ~/.bashrc

WORKDIR /nvmsim

ENTRYPOINT ["/bin/bash"]
