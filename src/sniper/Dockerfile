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
    g++-4.8 \
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
    libexpat-dev \
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

# Helper utilities
RUN apt-get update && apt-get install -y \
    gdb \
    git \
    && rm -rf /var/lib/apt/lists/*

# Add pinplay
ADD vendor/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux.tar.gz /opt

# Get Sniper 7.3
RUN cd /opt && git clone http://snipersim.org/download/68e4381a4e874444/git/sniper.git /opt/sniper-7.3

# Set environment values and alias
ENV SNIPER_HOME=/opt/sniper-7.3
ENV PIN_HOME=/opt/pinplay-drdebug-3.7-pin-3.7-97619-g0d0c92f4f-gcc-linux
RUN echo "alias run-sniper='/opt/sniper-7.3/run-sniper'" >> ~/.bashrc
RUN echo "alias test-sniper='cd /opt/sniper-7.3/test/fft; make run'" >> ~/.bashrc

# Add custom files
ADD src/sniper/dram_perf_model.cc /opt/sniper-7.3/common/performance_model/
ADD src/sniper/dram_perf_model_nvm.cc /opt/sniper-7.3/common/performance_model/
ADD src/sniper/dram_perf_model_nvm.h /opt/sniper-7.3/common/performance_model/
ADD src/sniper/base.cfg /opt/sniper-7.3/config/

# Make
RUN cd /opt/sniper-7.3/; make
