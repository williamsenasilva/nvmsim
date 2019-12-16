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
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - memory type NVM started\n");

   int client_socket;
   struct sockaddr_in servaddr;

   char hostname[] = "nvmain";
   char ip[100];

   getHostnameByIP(hostname, ip);
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - %s resolved to %s\n", hostname, ip);

   // socket create and varification
   client_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (client_socket == -1)
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - socket creation failed...\n");
      // exit(-1);
   }
   else
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - socket successfully created..\n");
   }
   bzero(&servaddr, sizeof(servaddr));

   // assign IP, PORT
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(ip);
   servaddr.sin_port = htons(PORT);

   // connect the client socket to server socket
   if (connect(client_socket, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0)
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - connection with the server failed...\n");
      // exit(-2);
   }
   else
   {
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] DramPerfModelNVM(...) - connected to the server..\n");
   }

   // function for chat
   // chat(client_socket);

   // close the socket
   close(client_socket);
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
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) <- (SubsecondTime pkt_time, UInt64 pkt_size, core_id_t requester, IntPtr address, DramCntlrInterface::access_t access_type, ShmemPerf *perf)\n");
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {pkt_time: %lldns}\n", (long long) pkt_time.getNS());
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {pkt_size: %lld}\n", (long long) pkt_size);
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {requester: %d}\n", requester);
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {address: %p}\n", (void *) address);
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {access_type: %d}\n", access_type);
   // pkt_size is in 'Bytes'
   // m_dram_bandwidth is in 'Bits per clock cycle'
   if ((!m_enabled) || (requester >= (core_id_t) Config::getSingleton()->getApplicationCores()))
   {
      return SubsecondTime::Zero();
   }

   SubsecondTime processing_time = m_dram_bandwidth.getRoundedLatency(8 * pkt_size); // bytes to bits
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {processing_time: %lld ns}\n", (long long) processing_time.getNS());

   // Compute Queue Delay
   SubsecondTime queue_delay;
   if (m_queue_model)
   {
      queue_delay = m_queue_model->computeQueueDelay(pkt_time, processing_time, requester);
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {queue_delay: %lld ns}\n", (long long) queue_delay.getNS());
   }
   else
   {
      queue_delay = SubsecondTime::Zero();
      printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {queue_delay: %lld ns}\n", (long long) queue_delay.getNS());
   }

   SubsecondTime access_latency = queue_delay + processing_time + m_dram_access_cost;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {access_latency: %lld ns}\n", (long long) access_latency.getNS());

   perf->updateTime(pkt_time);
   perf->updateTime(pkt_time + queue_delay, ShmemPerf::DRAM_QUEUE);
   perf->updateTime(pkt_time + queue_delay + processing_time, ShmemPerf::DRAM_BUS);
   perf->updateTime(pkt_time + queue_delay + processing_time + m_dram_access_cost, ShmemPerf::DRAM_DEVICE);

   // Update Memory Counters
   m_num_accesses++;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_num_accesses: %lld }\n", (long long) m_num_accesses);
   m_total_access_latency += access_latency;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_total_access_latency: %lld ns}\n", (long long) m_total_access_latency.getNS());
   m_total_queueing_delay += queue_delay;
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getAccessLatency(...) - {m_total_queueing_delay: %lld ns}\n", (long long) m_total_queueing_delay.getNS());

   return access_latency;
}

int DramPerfModelNVM::getHostnameByIP(char *hostname, char *ip)
{
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] getHostnameByIP(...) <- (char *hostname, char *ip)\n");
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

void DramPerfModelNVM::chat(int client_socket)
{
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] chat(...) <- (int client_socket)\n");
   char buffer[MAX];
   int write_res, read_res;
   for (int calls = 1; calls <= 100; calls++)
   {
      bzero(buffer, sizeof(buffer));
      sprintf(buffer, "%d", calls);
      write_res = write(client_socket, buffer, sizeof(buffer));
      if(write_res) 
      {
         bzero(buffer, sizeof(buffer));
         read_res = read(client_socket, buffer, sizeof(buffer));
         if(read_res)
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] chat(...) - from Server : %s\n", buffer);
         }
         else
         {
            printf("[NVMSIM] [dram_perf_model_nvm.cpp] chat(...) - error on read event\n");
         }
      }
      else
      {
         printf("[NVMSIM] [dram_perf_model_nvm.cpp] chat(...) - error on write event\n");
      }
      
   }

   bzero(buffer, sizeof(buffer));
   sprintf(buffer, "%s", "exit");
   write_res = write(client_socket, buffer, sizeof(buffer));
   printf("[NVMSIM] [dram_perf_model_nvm.cpp] chat(...) - client Exit...\n");
} 
