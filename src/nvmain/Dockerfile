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
ADD src/nvmain/traceReader/GenericTraceReader.h /opt/nvmain/traceReader
ADD src/nvmain/traceReader/TraceReaderFactory.cpp /opt/nvmain/traceReader
ADD src/nvmain/traceReader/SniperTrace /opt/nvmain/traceReader/SniperTrace
ADD src/nvmain/MemControl/FCFS.h /opt/nvmain/MemControl/FCFS
ADD src/nvmain/MemControl/FCFS.cpp /opt/nvmain/MemControl/FCFS
ADD src/nvmain/MemControl/FRFCFS.h /opt/nvmain/MemControl/FRFCFS
ADD src/nvmain/MemControl/FRFCFS.cpp /opt/nvmain/MemControl/FRFCFS
ADD src/nvmain/MemControl/FRFCFS-WQF.h /opt/nvmain/MemControl/FRFCFS-WQF
ADD src/nvmain/MemControl/FRFCFS-WQF.cpp /opt/nvmain/MemControl/FRFCFS-WQF
ADD src/nvmain/NVM/nvmain.h /opt/nvmain/NVM
ADD src/nvmain/NVM/nvmain.cpp /opt/nvmain/NVM
ADD src/nvmain/src/MemoryController.h /opt/nvmain/src
ADD src/nvmain/src/MemoryController.cpp /opt/nvmain/src
ADD src/nvmain/traceSim/traceMain.cpp /opt/nvmain/traceSim
ADD src/nvmain/SConscript /opt/nvmain

RUN cd /opt/nvmain && scons --build-type=fast
#RUN cd /opt/nvmain && scons --build-type=debug
#RUN cd /opt/nvmain && scons --build-type=prof
