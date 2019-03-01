FROM debian:9.4

# Update repo list and Install dependencies
RUN apt-get update && apt-get install -y \
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

ADD vendor/pinplay-drdebug-3.5-pin-3.5-97503-gac534ca30-gcc-linux.tar.bz2 /opt
ADD vendor/sniper-latest.tgz /opt

ENV PIN_HOME=/opt/pinplay-drdebug-3.5-pin-3.5-97503-gac534ca30-gcc-linux
ENV SNIPER_HOME=/opt/sniper-7.1

RUN echo 'installing sniper...'
RUN cd /opt/sniper-7.1/; make
RUN echo 'installing sniper... done'

RUN echo "alias run-sniper='/opt/sniper-7.1/run-sniper'" >> ~/.bashrc
RUN echo "alias test-sniper='cd /opt/sniper-7.1/test/fft; make run'" >> ~/.bashrc

WORKDIR /nvmsim

ENTRYPOINT ["/bin/bash"]