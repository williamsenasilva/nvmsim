# NVMSIM

## [1.0.0] - yyyy-mm-dd

#### Adicionado
- Dockerfile para imagem Sniper 7.2 com Ubuntu 18.04 mais aplicações auxiliares
- Dockerfile para imagem NVMain 2.0 com Ubuntu 18.04 mais aplicações auxiliares
- **dram_perf_model_nvm.cc** na pasta **sniper-7.2/common/performance_model/**
- **dram_perf_model_nvm.h** na pasta **sniper-7.2/common/performance_model/**

#### Modificado
- **sniper-7.2/common/performance_model/dram_perf_model.cc** para incluir **dram_perf_model_normal.h**
- **sniper-7.2/config/base.cfg** para setar **type = nvm** na seção **[perf_model/dram]**
