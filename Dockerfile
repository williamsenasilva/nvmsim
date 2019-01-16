FROM debian:9.5

# Update repo list and install dependencies
RUN apt-get update && apt-get install -y \
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
    vim \
    emacs

# Create shared folder
RUN mkdir -p /root/shared

# Install Sniper 
ADD shared/vendor/pinplay-drdebug-3.5-pin-3.5-97503-gac534ca30-gcc-linux.tar.bz2 /opt
ADD shared/vendor/sniper-latest.tgz /opt
ADD setup.sh /tmp
RUN chmod +x /tmp/setup.sh && /tmp/setup.sh

# Set WORKDIR
WORKDIR /root/shared

# Mount entrypoint
ENTRYPOINT ["/bin/bash"]

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