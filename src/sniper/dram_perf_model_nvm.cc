#include "dram_perf_model_nvm.h"
#include "simulator.h"
#include "config.h"
#include "config.hpp"
#include "stats.h"
#include "shmem_perf.h"

#define MAX 80
#define PORT 2000

DramPerfModelNVM::DramPerfModelNVM(core_id_t core_id, UInt32 cache_block_size) :
        DramPerfModel(core_id, cache_block_size),
        m_queue_model(NULL),
        m_dram_bandwidth(8 * Sim()->getCfg()->getFloat("perf_model/dram/per_controller_bandwidth")), // Convert bytes to bits
        m_total_queueing_delay(SubsecondTime::Zero()),
        m_total_access_latency(SubsecondTime::Zero())
{
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) <- (core_id, cache_block_size)\n");
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) <- (%d, %d)\n", core_id, cache_block_size);

   traceFile = "new-traceFile.nvt";
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - opening trace file %s... \n", traceFile.c_str( ));
   trace.open( traceFile.c_str( ) );
   if( !trace.is_open( ) )
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - opening trace file %s... error\n", traceFile.c_str( ));
      std::cerr << "Could not open trace file: " << traceFile << "!" << std::endl;
   }
   else
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - opening trace file %s... done\n", traceFile.c_str( ));
   }

   m_dram_access_cost = SubsecondTime::FS() * static_cast<uint64_t>(TimeConverter<float>::NStoFS(Sim()->getCfg()->getFloat("perf_model/dram/latency"))); // Operate in fs for higher precision before converting to uint64_t/SubsecondTime

   if (Sim()->getCfg()->getBool("perf_model/dram/queue_model/enabled"))
   {
      m_queue_model = QueueModel::create("dram-queue", core_id, Sim()->getCfg()->getString("perf_model/dram/queue_model/type"), m_dram_bandwidth.getRoundedLatency(8 * cache_block_size)); // bytes to bits
   }

   registerStatsMetric("dram", core_id, "total-access-latency", &m_total_access_latency);
   registerStatsMetric("dram", core_id, "total-queueing-delay", &m_total_queueing_delay);
}


DramPerfModelNVM::~DramPerfModelNVM()
{
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] ~DramPerfModelNVM()\n");
   if (m_queue_model)
   {
      delete m_queue_model;
      m_queue_model = NULL;
   }
}

SubsecondTime DramPerfModelNVM::getAccessLatency(SubsecondTime pkt_time, UInt64 pkt_size, core_id_t requester, IntPtr address, DramCntlrInterface::access_t access_type, ShmemPerf *perf)
{
   printf("\n[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) <- (pkt_time, pkt_size, requester, address, access_type, *perf)\n");
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) <- (%" PRIu64 " ns, %" PRIu64 ", %d, %p, %d, %p)\n", pkt_time.getNS(), pkt_size, requester, (void *) address, access_type, (void *) perf);
   // pkt_size is in 'Bytes'
   // m_dram_bandwidth is in 'Bits per clock cycle'
   if ((!m_enabled) || (requester >= (core_id_t) Config::getSingleton()->getApplicationCores()))
   {
      return SubsecondTime::Zero();
   }

   SubsecondTime processing_time = m_dram_bandwidth.getRoundedLatency(8 * pkt_size); // bytes to bits
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {processing_time: %" PRIu64 " ns}\n", processing_time.getNS());

   // Compute Queue Delay
   SubsecondTime queue_delay;
   if (m_queue_model)
   {
      queue_delay = m_queue_model->computeQueueDelay(pkt_time, processing_time, requester);
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {queue_delay: %" PRIu64 " ns}\n", queue_delay.getNS());
   }
   else
   {
      queue_delay = SubsecondTime::Zero();
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {queue_delay: %" PRIu64 " ns}\n", queue_delay.getNS());
   }

   SubsecondTime access_latency = queue_delay + processing_time + m_dram_access_cost;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {access_latency: %" PRIu64 " ns}\n", access_latency.getNS());

   perf->updateTime(pkt_time);
   perf->updateTime(pkt_time + queue_delay, ShmemPerf::DRAM_QUEUE);
   perf->updateTime(pkt_time + queue_delay + processing_time, ShmemPerf::DRAM_BUS);
   perf->updateTime(pkt_time + queue_delay + processing_time + m_dram_access_cost, ShmemPerf::DRAM_DEVICE);

   // Update Memory Counters
   m_num_accesses++;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_num_accesses: %lld }\n", (long long) m_num_accesses);
   m_total_access_latency += access_latency;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_total_access_latency: %" PRIu64 " ns}\n", m_total_access_latency.getNS());
   m_total_queueing_delay += queue_delay;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_total_queueing_delay: %" PRIu64 " ns}\n", m_total_queueing_delay.getNS());

   return access_latency;
}

