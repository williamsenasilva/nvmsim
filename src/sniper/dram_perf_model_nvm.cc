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
   m_dram_access_cost = SubsecondTime::FS() * static_cast<uint64_t>(TimeConverter<float>::NStoFS(Sim()->getCfg()->getFloat("perf_model/dram/latency"))); // Operate in fs for higher precision before converting to uint64_t/SubsecondTime

   if (Sim()->getCfg()->getBool("perf_model/dram/queue_model/enabled"))
   {
      m_queue_model = QueueModel::create("dram-queue", core_id, Sim()->getCfg()->getString("perf_model/dram/queue_model/type"), m_dram_bandwidth.getRoundedLatency(8 * cache_block_size)); // bytes to bits
   }

   registerStatsMetric("dram", core_id, "total-access-latency", &m_total_access_latency);
   registerStatsMetric("dram", core_id, "total-queueing-delay", &m_total_queueing_delay);
   printf("[NVMSIM] Memory type NVM started\n");

   int client_socket;
   struct sockaddr_in servaddr;

   char hostname[] = "nvmain";
   char ip[100];

   getHostnameByIP(hostname, ip);
   printf("%s resolved to %s\n", hostname, ip);

   // socket create and varification
   client_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (client_socket == -1)
   {
      printf("socket creation failed...\n");
      exit(0);
   }
   else
   {
      printf("Socket successfully created..\n");
   }
   bzero(&servaddr, sizeof(servaddr));

   // assign IP, PORT
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(ip);
   servaddr.sin_port = htons(PORT);

   // connect the client socket to server socket
   if (connect(client_socket, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0)
   {
      printf("connection with the server failed...\n");
      exit(0);
   }
   else
   {
      printf("connected to the server..\n");
   }

   // function for chat
   chat(client_socket);

   // close the socket
   close(client_socket);
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

   SubsecondTime access_latency = queue_delay + processing_time + m_dram_access_cost;

   perf->updateTime(pkt_time);
   perf->updateTime(pkt_time + queue_delay, ShmemPerf::DRAM_QUEUE);
   perf->updateTime(pkt_time + queue_delay + processing_time, ShmemPerf::DRAM_BUS);
   perf->updateTime(pkt_time + queue_delay + processing_time + m_dram_access_cost, ShmemPerf::DRAM_DEVICE);

   // Update Memory Counters
   m_num_accesses++;
   m_total_access_latency += access_latency;
   m_total_queueing_delay += queue_delay;

   printf("[NVMSIM] {num_accesses,access_latency,total_access_latency}: %"PRIu64",%"PRIu64"ns,%"PRIu64"ns\n", m_num_accesses, access_latency.getNS(), m_total_access_latency.getNS());
   return access_latency;
}

int DramPerfModelNVM::getHostnameByIP(char *hostname, char *ip)
{
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
   char buffer[MAX];

   for (int calls = 1; calls <= 100; calls++)
   {
      bzero(buffer, sizeof(buffer));
      sprintf(buffer, "%d", calls);
      write(client_socket, buffer, sizeof(buffer));
      bzero(buffer, sizeof(buffer));
      read(client_socket, buffer, sizeof(buffer));
      printf("From Server : %s\n", buffer);
   }

   bzero(buffer, sizeof(buffer));
   sprintf(buffer, "%s", "exit");
   write(client_socket, buffer, sizeof(buffer));
   printf("Client Exit...\n");
} 
