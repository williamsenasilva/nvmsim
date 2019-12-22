FROM ubuntu:18.04
MAINTAINER William Sena Silva

# For NVMain
RUN apt-get update && apt-get install -y \
    build-essential \
    mercurial \
    scons \
    && rm -rf /var/lib/apt/lists/*

RUN cd /opt && hg clone https://bitbucket.org/mrp5060/nvmain

# remove gem5 dependency
RUN sed -i -e 's/from gem5_scons import Transform/#from gem5_scons import Transform/g' /opt/nvmain/SConscript

RUN cd /opt/nvmain && scons --build-type=fast
RUN cd /opt/nvmain && scons --build-type=debug
RUN cd /opt/nvmain && scons --build-type=prof

# Set environment values and alias
ENV NVMSIM="sniper"

EXPOSE 2000
WORKDIR /opt/nvmain
ENTRYPOINT ["/bin/bash"]

# Add custom files and make
ADD src/nvmain/TraceLine.cpp /opt/nvmain/traceReader 
ADD src/nvmain/traceMain.cpp /opt/nvmain/traceSim
ADD src/nvmain/TraceReaderFactory.cpp /opt/nvmain/traceReader

ADD src/nvmain/traceReader/SniperTrace /opt/nvmain/traceReader/SniperTrace
ADD src/nvmain/SConscript /opt/nvmain

# remove gem5 dependency
RUN cd /opt/nvmain && scons --build-type=fast
