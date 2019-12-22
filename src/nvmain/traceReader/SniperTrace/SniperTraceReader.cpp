/*******************************************************************************
* Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
* Department of Computer Science and Engineering, The Pennsylvania State University
* All rights reserved.
* 
* This source code is part of Sniper - A cycle accurate timing, bit accurate
* energy simulator for both volatile (e.g., DRAM) and non-volatile memory
* (e.g., PCRAM). The source code is free and you can redistribute and/or
* modify it by providing that the following conditions are met:
* 
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
* 
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* Author list: 
*   Matt Poremba    ( Email: mrp5060 at psu dot edu 
*                     Website: http://www.cse.psu.edu/~poremba/ )
*******************************************************************************/

#include "traceReader/SniperTrace/SniperTraceReader.h"
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

#include <arpa/inet.h>
#include <errno.h>

#define MAX 80
#define PORT 7200

using namespace NVM;

SniperTraceReader::SniperTraceReader( )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] SniperTraceReader( )\n");
    traceFile = "";

    traceVersion = 0;
    readVersion = false;
    StartSniperCommunication();
}

SniperTraceReader::~SniperTraceReader( )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] ~SniperTraceReader( )\n");
    if( trace.is_open( ) )
        trace.close( );
}

void SniperTraceReader::SetTraceFile( std::string file )
{
    traceFile = file;
}

std::string SniperTraceReader::GetTraceFile( )
{
    return traceFile;
}

/*
 *  This trace is printed from nvmain.cpp. The format is:
 *
 *  CYCLE OP ADDRESS DATA THREADID
 */
bool SniperTraceReader::GetNextAccess( TraceLine *nextAccess )
{
    /* If there is no trace file, we can't do anything. */
    if( traceFile == "" )
    {
        std::cerr << "No trace file specified!" << std::endl;
        return false;
    }

    /* If the trace file is not open, open it if possible. */
    if( !trace.is_open( ) )
    {
        trace.open( traceFile.c_str( ) );
        if( !trace.is_open( ) )
        {
            std::cerr << "Could not open trace file: " << traceFile << "!" << std::endl;
            return false;
        }
    }

    if( !nvmain_socket )
    {
        printf("[NVMSIM] [SniperTraceReader.cpp] SniperTraceReader( ) - reopen nvmain socket\n");
        StartSniperCommunication();
    }

    std::string fullLine;

    /* We will read in a full line and fill in these values */
    unsigned int cycle = 0;
    OpType operation = READ;
    uint64_t address;
    NVMDataBlock dataBlock;
    NVMDataBlock oldDataBlock;
    unsigned int threadId = 0;
    
    /* There are no more lines in the trace... Send back a "dummy" line */
    getline( trace, fullLine );
    if( trace.eof( ) )
    {
        NVMAddress nAddress;
        nAddress.SetPhysicalAddress( 0xDEADC0DEDEADBEEFULL );
        nextAccess->SetLine( nAddress, NOP, 0, dataBlock, oldDataBlock, 0 );
        std::cout << "SniperTraceReader: Reached EOF!" << std::endl;
        return false;
    }

    if( !readVersion )
    {
        if( fullLine.substr( 0, 4 ) == "NVMV" )
        {
            std::string versionString = fullLine.substr( 4, std::string::npos );
            traceVersion = atoi( versionString.c_str( ) );
        }

        readVersion = true;
        getline( trace, fullLine );
    }
    
    std::istringstream lineStream( fullLine );
    std::string field;
    unsigned char fieldId = 0;
    
    /*
     *  Again, the format is : CYCLE OP ADDRESS DATA THREADID
     *  So the field ids are :   0    1    2      3      4
     */
    while( getline( lineStream, field, ' ' ) )
    {
        if( field != "" )
        {
            if( fieldId == 0 )
                cycle = atoi( field.c_str( ) );
            else if( fieldId == 1 )
            {
                if( field == "R" )
                    operation = READ;
                else if( field == "W" )
                    operation = WRITE;
                else
                    std::cout << "Warning: Unknown operation `" 
                        << field << "'" << std::endl;
            }
            else if( fieldId == 2 )
            {
                std::stringstream fmat;

                fmat << std::hex << field;
                fmat >> address;
            }
            else if( fieldId == 3 )
            {
                int byte;
                int start, end;

                /* Assumes 64-byte memory words.... */
                // TODO: Drop assumption and use field.length()/2 bytes
                assert(sizeof(uint64_t)*8 == 64);
                assert(field.length() == 128); // 1 char per 4 bits

                dataBlock.SetSize( 64 );

                uint32_t *rawData = reinterpret_cast<uint32_t*>(dataBlock.rawData);
                memset(rawData, 0, 64);
                
                for( byte = 0; byte < 16; byte++ )
                {
                    std::stringstream fmat;

                    end = 8*byte + 8;
                    start = 8*byte;

                    fmat << std::hex << field.substr( start, end - start );
                    fmat >> rawData[byte];
                    rawData[byte] = htonl( rawData[byte] );
                }
            }
            else if( fieldId == 4 )
            {
                if( traceVersion == 0 )
                {
                    threadId = atoi( field.c_str( ) );

                    /* Zero out old data in 1.0 trace format. */
                    oldDataBlock.SetSize( 64 );

                    uint64_t *rawData = reinterpret_cast<uint64_t*>(oldDataBlock.rawData);
                    memset(rawData, 0, 64);
                }
                else
                {
                    int byte;
                    int start, end;

                    /* Assumes 64-byte memory words.... */
                    // TODO: Drop assumption and use field.length()/2 bytes
                    assert(sizeof(uint64_t)*8 == 64);
                    assert(field.length() == 128); // 1 char per 4 bits

                    oldDataBlock.SetSize( 64 );

                    uint32_t *rawData = reinterpret_cast<uint32_t*>(oldDataBlock.rawData);
                    memset(rawData, 0, 64);
                    
                    for( byte = 0; byte < 16; byte++ )
                    {
                        std::stringstream fmat;

                        end = 8*byte + 8;
                        start = 8*byte;

                        fmat << std::hex << field.substr( start, end - start );
                        fmat >> rawData[byte];
                        rawData[byte] = htonl( rawData[byte] );
                    }
                }
            }
            else if( fieldId == 5 )
            {
                assert( traceVersion != 0 );
                threadId = atoi( field.c_str( ) );
            }
            
            fieldId++;
        }
    }

    static unsigned int linenum = 0;

    linenum++;

    if( operation != READ && operation != WRITE )
        std::cout << "SniperTraceReader: Unknown Operation: " << operation 
            << "Line number is " << linenum << ". Full Line is \"" << fullLine 
            << "\"" << std::endl;

    /*
     *  Set the line parameters.
     */
    NVMAddress nAddress;

    nAddress.SetPhysicalAddress( address );

    nextAccess->SetLine( nAddress, operation, cycle, dataBlock, oldDataBlock, threadId );

    return true;
}

/* 
 * Get the next N accesses to main memory. Called GetNextAccess N times and 
 * places the return values into a vector of TraceLine pointers.
 */
int SniperTraceReader::GetNextNAccesses( unsigned int N, 
                                   std::vector<TraceLine *> *nextAccesses )
{
    int successes;
    class TraceLine *nextLine;

    /* Keep track of the actual number of accesses returned. */
    successes = 0;

    /* Loop N times, calling GetNextAccess each iteration. */
    for( unsigned int i = 0; i < N; i++ )
    {
        /* We need a new TraceLine so the old values are not overwritten. */
        nextLine = new TraceLine( );

        /* Get the next access and place it in "nextLine" */
        if( GetNextAccess( nextLine ) )
        {
            /* Push next line into the vector. */
            nextAccesses->push_back( nextLine );

            successes++;
        }
    }

    return successes;
}

bool SniperTraceReader::StartSniperCommunication( ) 
{
    printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( )\n");
    struct sockaddr_in nvmain_address;
    // socket create and verification 
    // socket(int domain, int type, int protocol)
    nvmain_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (nvmain_socket == -1) 
    { 
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - socket creation failed.\n"); 
        return false;
    } 
    else 
    {
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - socket successfully created.\n"); 
    }
    
    // clear address structure
    bzero(&nvmain_address, sizeof(nvmain_address)); 

    // assign IP, PORT 
    nvmain_address.sin_family = AF_INET; 
    nvmain_address.sin_addr.s_addr = INADDR_ANY; 
    nvmain_address.sin_port = htons(PORT); 

    // Binding newly created socket to given IP and verification 
    // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
    if (bind(nvmain_socket, (struct sockaddr*) &nvmain_address, sizeof(nvmain_address)) < 0) 
    { 
        perror("ERROR on binding");
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - socket bind failed.\n"); 
        return false;
    } 
    else
    {
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - socket successfully binded.\n"); 
    }

    // Now nvmain is ready to listen and verification 
    if ((listen(nvmain_socket, 5)) != 0) 
    { 
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - listen failed.\n"); 
        return false;
    } 
    else
    {
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - NVMain listening.\n"); 
    }

    return true;
}

bool SniperTraceReader::Accept()
{
    struct sockaddr_in sniper_address; 
    socklen_t sniper_len;
    sniper_len = sizeof(sniper_address);

    // Accept the data packet from sniper and verification 
    new_nvmain_socket = accept(nvmain_socket, (struct sockaddr*) &sniper_address, &sniper_len); 
    if (new_nvmain_socket < 0) 
    { 
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - nvmain acccept failed.\n"); 
        return false;
    } 
    else
    {
        printf("[NVMSIM] [SniperTraceReader.cpp] StartSniperCommunication( ) - nvmain acccept the sniper.\n"); 
    }
    return true;
}

bool SniperTraceReader::FinishSniperCommunication()
{
    close(this->nvmain_socket); 
    return true;
}

int SniperTraceReader::GetHostnameByIP(char *hostname, char *ip)
{
   printf("[NVMSIM] [SniperTraceReader.cpp] getHostnameByIP(...) <- (char *hostname, char *ip)\n");
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

void SniperTraceReader::Chat()
{
   printf("[NVMSIM] [SniperTraceReader.cpp] chat()\n");
   char buffer[MAX];
   int write_res, read_res;
   for (int calls = 1; calls <= 100; calls++)
   {
      bzero(buffer, sizeof(buffer));
      sprintf(buffer, "%d", calls);
      write_res = write(this->nvmain_socket, buffer, sizeof(buffer));
      if(write_res) 
      {
         bzero(buffer, sizeof(buffer));
         read_res = read(this->nvmain_socket, buffer, sizeof(buffer));
         if(read_res)
         {
            printf("[NVMSIM] [SniperTraceReader.cpp] Chat() - from NVMain : %s\n", buffer);
         }
         else
         {
            printf("[NVMSIM] [SniperTraceReader.cpp] Chat() - error on read event\n");
         }
      }
      else
      {
         printf("[NVMSIM] [SniperTraceReader.cpp] Chat() - error on write event\n");
      }
   }

   bzero(buffer, sizeof(buffer));
   sprintf(buffer, "%s", "exit");
   write_res = write(this->nvmain_socket, buffer, sizeof(buffer));
   printf("[NVMSIM] [SniperTraceReader.cpp] Chat() - sniper Exit...\n");
} 