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

ADD vendor/sniper-latest.tgz /opt
ENV SNIPER_HOME=/opt/sniper-7.2
ADD vendor/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux.tar.gz /opt
ENV PIN_HOME=/opt/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux
RUN cd /opt/sniper-7.2/; make

# For NVMain
RUN apt-get update && apt-get install -y \
    mercurial \
    scons \
    && rm -rf /var/lib/apt/lists/*

RUN cd /opt && hg clone https://bitbucket.org/mrp5060/nvmain

# remove gem5 dependency
RUN sed -i -e 's/from gem5_scons import Transform/#from gem5_scons import Transform/g' /opt/nvmain/SConscript

RUN cd /opt/nvmain && scons --build-type=fast
RUN cd /opt/nvmain && scons --build-type=debug
RUN cd /opt/nvmain && scons --build-type=prof

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

# Config environment
RUN echo "alias run-sniper='/opt/sniper-7.2/run-sniper'" >> ~/.bashrc
RUN echo "alias test-sniper='cd /opt/sniper-7.2/test/fft; make run'" >> ~/.bashrc

WORKDIR /nvmsim

ENTRYPOINT ["/bin/bash"]
