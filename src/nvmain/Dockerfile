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
ADD src/server.c /opt/nvmain
RUN gcc /opt/nvmain/server.c -o /opt/nvmain/server

ADD src/nvmain/nvmain.cpp /opt/nvmain
ADD src/nvmain/NullInterface.cpp /opt/nvmain/SimInterface/NullInterface
ADD src/nvmain/MemoryControllerFactory.cpp /opt/nvmain/MemControl
ADD src/nvmain/TraceLine.cpp /opt/nvmain/traceReader
ADD src/nvmain/traceMain.cpp /opt/nvmain/traceSim
ADD src/nvmain/TraceReaderFactory.cpp /opt/nvmain/traceReader
ADD src/nvmain/RubyTraceReader.cpp /opt/nvmain/traceReader/RubyTrace
ADD src/nvmain/NVMainTraceReader.cpp /opt/nvmain/traceReader/NVMainTrace

ADD src/nvmain/Simulators/sniper /opt/nvmain/Simulators/sniper
ADD src/nvmain/SimInterface/SniperInterface /opt/nvmain/SimInterface/SniperInterface
ADD src/nvmain/SConscript /opt/nvmain
ADD src/nvmain/SConstruct /opt/nvmain

# remove gem5 dependency
RUN cd /opt/nvmain && scons --build-type=fast
