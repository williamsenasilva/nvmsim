FROM ubuntu:18.04
MAINTAINER William Sena Silva

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

# For Sniper
RUN apt-get update && apt-get install -y \
    automake \
    build-essential \
    curl \
    git \
    g++-6 \
    libboost-dev \
    libbz2-dev \
    libsqlite3-dev \
    zlib1g-dev \
    wget \
    && rm -rf /var/lib/apt/lists/*

# For RISC-V Tools
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

# For SPEC CPU2006
RUN apt-get update && apt-get install -y \
    gfortran

# For user utilities
RUN apt-get update && apt-get install -y \
    gdb \
    procps \
    vim \
    emacs \
    && rm -rf /var/lib/apt/lists/*

# Add vendor sources and make
ADD vendor/sniper-7.2.tgz /opt
ADD vendor/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux.tar.gz /opt

# Set environment values and alias
ENV SNIPER_HOME=/opt/sniper-7.2
ENV PIN_HOME=/opt/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux
RUN echo "alias run-sniper='/opt/sniper-7.2/run-sniper'" >> ~/.bashrc
RUN echo "alias test-sniper='cd /opt/sniper-7.2/test/fft; make run'" >> ~/.bashrc

RUN cd /opt/sniper-7.2/; make
RUN mkdir -p /nvmsim/logs

EXPOSE 7200
WORKDIR /nvmsim
ENTRYPOINT ["/bin/bash"]

# Add custom files and make
ADD src/sniper/dram_perf_model.cc /opt/sniper-7.2/common/performance_model/
ADD src/sniper/dram_perf_model_nvm.cc /opt/sniper-7.2/common/performance_model/
ADD src/sniper/dram_perf_model_nvm.h /opt/sniper-7.2/common/performance_model/
ADD src/sniper/base.cfg /opt/sniper-7.2/config/
RUN cd /opt/sniper-7.2/; make

ADD src/client.c /opt/sniper-7.2
RUN gcc /opt/sniper-7.2/client.c -o /opt/sniper-7.2/client
