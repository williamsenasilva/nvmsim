# NVMSIM

## [1.0.0] - yyyy-mm-dd

#### Adicionado
- Dockerfile para imagem Sniper 7.3 com Ubuntu 18.04 mais aplicações auxiliares
- Dockerfile para imagem NVMain 2.0 com Ubuntu 18.04 mais aplicações auxiliares

- **sniper-7.3/common/performance_model/dram_perf_model_nvm.cc**
- **sniper-7.3/common/performance_model/dram_perf_model_nvm.h**
- **nvmain/traceReader/SniperTrace/SniperTraceReader.cpp**
- **nvmain/traceReader/SniperTrace/SniperTraceReader.h**

#### Modificado
- **sniper-7.3/common/performance_model/dram_perf_model.cc** para incluir **dram_perf_model_nvm.h**
- **sniper-7.3/config/base.cfg** para setar **type = nvm** na seção **[perf_model/dram]**
- **nvmain/TraceReaderFactory.cpp** para incluir **SniperTraceReader.h** 
- **nvmain/SConscript** para configurar ambiente Sniper 
