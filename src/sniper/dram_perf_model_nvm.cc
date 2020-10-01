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

   tracefile = "/mnt/nvmsim/tracefile";
   mkfifo(tracefile.c_str(), 0666);

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
   if (m_queue_model)
   {
      delete m_queue_model;
      m_queue_model = NULL;
   }
}

SubsecondTime DramPerfModelNVM::getAccessLatency(SubsecondTime pkt_time, UInt64 pkt_size, core_id_t requester, IntPtr address, DramCntlrInterface::access_t access_type, ShmemPerf *perf)
{
   printf("[NVMSIM][DEBUG] getAccessLatency(pkt_time: %" PRIu64 ", pkt_size: %" PRIu64 ", requester: %d, address: %p, access_type: %d, perf: %p)\n", pkt_time.getNS(), pkt_size, requester, (void *) address, access_type, (void *) perf);

   // pkt_size is in 'Bytes'
   // m_dram_bandwidth is in 'Bits per clock cycle'
   if ((!m_enabled) || (requester >= (core_id_t) Config::getSingleton()->getApplicationCores()))
   {
      return SubsecondTime::Zero();
   }

   SubsecondTime processing_time = m_dram_bandwidth.getRoundedLatency(8 * pkt_size); // bytes to bits

   // Compute Queue Delay
   SubsecondTime queue_delay;
   if (m_queue_model)
   {
      queue_delay = m_queue_model->computeQueueDelay(pkt_time, processing_time, requester);
   }
   else
   {
      queue_delay = SubsecondTime::Zero();
   }

   if(access_type == 0 || access_type == 1)
   {
      int fd, response;
      fd = open(tracefile.c_str(), O_WRONLY);
      if(fd != -1)
      {
         std::string message_to_nvmain;
         std::string message_from_nvmain;
         std::ostringstream ss_m_num_accesses;
         std::ostringstream ss_address;
         std::ostringstream ss_requester;
         std::ostringstream ss_perf;

         ss_m_num_accesses << m_num_accesses;
         ss_address << (void const *) address;
         ss_requester << requester;
         ss_perf << (void const *) perf;
         message_to_nvmain = "";
         message_to_nvmain += ss_m_num_accesses.str();
         message_to_nvmain += " ";
         message_to_nvmain += access_type == 0 ? 'R' : 'W';
         message_to_nvmain += " ";
         message_to_nvmain += ss_address.str();
         message_to_nvmain += " ";
         message_to_nvmain += "wdffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
         // message_to_nvmain += ss_perf.str();
         message_to_nvmain += " ";
         message_to_nvmain += ss_requester.str();
         printf("[NVMSIM][INFO ] send: %s\n", message_to_nvmain.c_str());
         message_to_nvmain += "\n";
         //printf("[NVMSIM][DEBUG ] block size: %d\n", getCacheBlockSize());
         response = write(fd, message_to_nvmain.c_str(), message_to_nvmain.length());
         if(!response)
         {
            printf("[NVMSIM][ERROR] Error on writing message\n");
         }
         close(fd);

         fd = open(tracefile.c_str(), O_RDONLY);
         if(fd != -1)
         {
            char buffer[10];
            response = read(fd, buffer, 10);
            if(response)
            {
               message_from_nvmain = "";
               int index = 0;
               while (buffer[ index ] != '\n')
                  message_from_nvmain += buffer[ index++ ];

               printf("[NVMSIM][INFO ] receive: %s\n", message_from_nvmain.c_str());
            }
            else
            {
               printf("[NVMSIM][ERROR] Error on reading message\n");
            }
            close(fd);
         }
      }
   }

   SubsecondTime access_latency = queue_delay + processing_time + m_dram_access_cost;

   perf->updateTime(pkt_time);
   perf->updateTime(pkt_time + queue_delay, ShmemPerf::DRAM_QUEUE);
   perf->updateTime(pkt_time + queue_delay + processing_time, ShmemPerf::DRAM_BUS);
   perf->updateTime(pkt_time + queue_delay + processing_time + m_dram_access_cost, ShmemPerf::DRAM_DEVICE);

   // Update Memory Counters
   m_num_accesses++;
   m_total_access_latency += access_latency;
   m_total_queueing_delay += queue_delay;

   // printf("[NVMSIM] DramPerfModelNVM::getAccessLatency(...) - {processing_time: %" PRIu64 " ns}\n", processing_time.getNS());
   // printf("[NVMSIM] DramPerfModelNVM::getAccessLatency(...) - {m_num_accesses: %" PRIu64 "}\n", m_num_accesses);
   // printf("[NVMSIM] DramPerfModelNVM::getAccessLatency(...) - {queue_delay: %" PRIu64 " ns}\n", queue_delay.getNS());
   // printf("[NVMSIM] DramPerfModelNVM::getAccessLatency(...) - {m_total_queueing_delay: %" PRIu64 " ns}\n", m_total_queueing_delay.getNS());
   // printf("[NVMSIM] DramPerfModelNVM::getAccessLatency(...) - {access_latency: %" PRIu64 " ns}\n", access_latency.getNS());
   // printf("[NVMSIM] DramPerfModelNVM::getAccessLatency(...) - {m_total_access_latency: %" PRIu64 " ns}\n", m_total_access_latency.getNS());
   printf("[NVMSIM][DEBUG] getAccessLatency(...) - return access_latency: %" PRIu64 "\n", access_latency.getNS());
   return access_latency;
}

