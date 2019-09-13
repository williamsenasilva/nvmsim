FROM ubuntu:18.04

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

WORKDIR /opt/nvmain

ENTRYPOINT ["/bin/bash"]
