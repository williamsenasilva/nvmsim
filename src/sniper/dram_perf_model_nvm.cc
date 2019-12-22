#include "dram_perf_model_nvm.h"
#include "simulator.h"
#include "config.h"
#include "config.hpp"
#include "stats.h"
#include "shmem_perf.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAX 80
#define PORT 2000

DramPerfModelNVM::DramPerfModelNVM(core_id_t core_id, UInt32 cache_block_size) :
        DramPerfModel(core_id, cache_block_size),
        m_queue_model(NULL),
        m_dram_bandwidth(8 * Sim()->getCfg()->getFloat("perf_model/dram/per_controller_bandwidth")), // Convert bytes to bits
        m_total_queueing_delay(SubsecondTime::Zero()),
        m_total_access_latency(SubsecondTime::Zero())
{
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) <- (core_id_t core_id, UInt32 cache_block_size)\n");
   m_dram_access_cost = SubsecondTime::FS() * static_cast<uint64_t>(TimeConverter<float>::NStoFS(Sim()->getCfg()->getFloat("perf_model/dram/latency"))); // Operate in fs for higher precision before converting to uint64_t/SubsecondTime

   if (Sim()->getCfg()->getBool("perf_model/dram/queue_model/enabled"))
   {
      m_queue_model = QueueModel::create("dram-queue", core_id, Sim()->getCfg()->getString("perf_model/dram/queue_model/type"), m_dram_bandwidth.getRoundedLatency(8 * cache_block_size)); // bytes to bits
   }

   registerStatsMetric("dram", core_id, "total-access-latency", &m_total_access_latency);
   registerStatsMetric("dram", core_id, "total-queueing-delay", &m_total_queueing_delay);

   StartNVMainCommunication();
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

bool DramPerfModelNVM::StartNVMainCommunication() 
{
   printf("\n[NVMSIM] [dram_perf_model_nvm.cpp] StartNVMainCommunication()\n");
   struct sockaddr_in nvmain_address;
   char hostname[] = "nvmain";
   char ip[100];

   GetHostnameByIP(hostname, ip);
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - %s resolved to %s\n", hostname, ip);

   // socket create and varification
   this->sniper_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (this->sniper_socket == -1)
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - socket creation failed\n");
      exit(1);
   }
   else
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - socket successfully created\n");
   }
   bzero(&nvmain_address, sizeof(nvmain_address));

   // assign IP, PORT
   nvmain_address.sin_family = AF_INET;
   nvmain_address.sin_addr.s_addr = inet_addr(ip);
   nvmain_address.sin_port = htons(PORT);

   // connect the sniper socket to nvmain socket
   if (connect(this->sniper_socket, (struct sockaddr *) &nvmain_address, sizeof(nvmain_address)) != 0)
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - connection with the nvmain failed...\n");
      exit(-1);
   }
   else
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - connected to the nvmain..\n");
   }

   return true;
}

bool DramPerfModelNVM::FinishNVMainCommunication()
{
   printf("\n[NVMSIM] [dram_perf_model_nvm.cpp] FinishNVMainCommunication()\n");
   close(this->sniper_socket);
   return true;
} 

int DramPerfModelNVM::GetHostnameByIP(char *hostname, char *ip)
{
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] GetHostnameByIP(...) <- (hostname, ip)\n");
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] GetHostnameByIP(...) <- (%s, %p)\n", hostname, (void *) ip);
   struct hostent *he;
   struct in_addr **addr_list;
   int i;

   if ((he = gethostbyname(hostname)) == NULL)
   {
      // get the host info
      herror("gethostbyname");
      return 1;
   }

   addr_list = (struct in_addr **) he->h_addr_list;

   for (i = 0; addr_list[i] != NULL; i++)
   {
      //Return the first one;
      strcpy(ip, inet_ntoa(*addr_list[i]));
      return 0;
   }

   return 1;
}

void DramPerfModelNVM::Chat()
{
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] Chat()\n");
   char buffer[MAX];
   int write_res, read_res;
   for (int calls = 1; calls <= 100; calls++)
   {
      bzero(buffer, sizeof(buffer));
      sprintf(buffer, "%d", calls);
      write_res = write(this->sniper_socket, buffer, sizeof(buffer));
      if(write_res) 
      {
         bzero(buffer, sizeof(buffer));
         read_res = read(this->sniper_socket, buffer, sizeof(buffer));
         if(read_res)
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] Chat() - from Server : %s\n", buffer);
         }
         else
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] Chat() - error on read event\n");
         }
      }
      else
      {
         printf("[NVMSIM] [dram_perf_model_nvm.cpp] Chat() - error on write event\n");
      }
      
   }

   bzero(buffer, sizeof(buffer));
   sprintf(buffer, "%s", "exit");
   write_res = write(this->sniper_socket, buffer, sizeof(buffer));
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] Chat() - sniper Exit...\n");
} 
