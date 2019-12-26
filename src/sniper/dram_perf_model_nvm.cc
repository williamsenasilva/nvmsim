#include "dram_perf_model_nvm.h"
#include "simulator.h"
#include "config.h"
#include "config.hpp"
#include "stats.h"
#include "shmem_perf.h"

#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

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

   fifofile = "/tmp/nvmsim-fifofile";
   
   mkfifo(fifofile.c_str(), 0666);

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

   printf("[NVMSIM]\n");
   if(access_type == 0 || access_type == 1)
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - opening fifofile to write...\n");
      int fd, response;
      fd = open(fifofile.c_str(), O_WRONLY);
      if(fd != -1)
      {
         printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - opening fifofile to write... done\n");
         
         std::string message_to_nvmain;
         char message_from_nvmain[10];
         std::ostringstream ss_m_num_accesses;
         std::ostringstream ss_address;
         std::ostringstream ss_requester;
         ss_m_num_accesses << m_num_accesses;
         ss_address << (void const *) address;
         ss_requester << requester;
         message_to_nvmain = "";
         message_to_nvmain += ss_m_num_accesses.str();
         message_to_nvmain += " ";
         message_to_nvmain += access_type == 0 ? 'R' : 'W';
         message_to_nvmain += " ";
         message_to_nvmain += ss_address.str();
         message_to_nvmain += " ";
         message_to_nvmain += "data";
         message_to_nvmain += " ";
         message_to_nvmain += ss_requester.str();
         
         response = write(fd, message_to_nvmain.c_str(), message_to_nvmain.length()); 
         if(response)
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {message_to_nvmain: %s}\n", message_to_nvmain.c_str());
         }
         else
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - error on writing message\n");
         }
         close(fd); 
         
         printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - opening fifofile to read...\n");
         fd = open(fifofile.c_str(), O_RDONLY);
         if(fd != -1)
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - opening fifofile to read... done\n");
            
            response = read(fd, message_from_nvmain, sizeof(message_from_nvmain));
            if(response)
            {
               printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {message_from_nvmain: %s}\n", message_from_nvmain);
            }
            else
            {
               printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - error on reading message\n");
            }
            close(fd); 
         }
         else
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - opening fifofile to read... error\n");
         }
         
      }
      else
      {
         printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - opening fifofile to write... error\n");
      }
   }

   SubsecondTime access_latency = queue_delay + processing_time + m_dram_access_cost;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {access_latency: %" PRIu64 " ns}\n", access_latency.getNS());

   perf->updateTime(pkt_time);
   perf->updateTime(pkt_time + queue_delay, ShmemPerf::DRAM_QUEUE);
   perf->updateTime(pkt_time + queue_delay + processing_time, ShmemPerf::DRAM_BUS);
   perf->updateTime(pkt_time + queue_delay + processing_time + m_dram_access_cost, ShmemPerf::DRAM_DEVICE);

   // Update Memory Counters
   m_num_accesses++;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_num_accesses: %" PRIu64 "}\n", m_num_accesses);
   m_total_access_latency += access_latency;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_total_access_latency: %" PRIu64 " ns}\n", m_total_access_latency.getNS());
   m_total_queueing_delay += queue_delay;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_total_queueing_delay: %" PRIu64 " ns}\n", m_total_queueing_delay.getNS());

   return access_latency;
}

