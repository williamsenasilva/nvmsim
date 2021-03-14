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
#include <arpa/inet.h>

#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

#include <inttypes.h>

#define MAX 80

using namespace NVM;

SniperTraceReader::SniperTraceReader( )
{
    tracefile = "";

    traceVersion = 0;
    readVersion = false;
}

SniperTraceReader::~SniperTraceReader( )
{
    if( trace.is_open( ) )
        trace.close( );
}

void SniperTraceReader::SetTraceFile( std::string file )
{
    printf("[NVMSIM][INFO ][TFILE] set trace file: %s\n", file.c_str());
    tracefile = file;
    mkfifo(tracefile.c_str(), 0666);
}

std::string SniperTraceReader::GetTraceFile( )
{
    return tracefile;
}

/*
 *  This trace is printed from nvmain.cpp. The format is:
 *
 *  CYCLE OP ADDRESS DATA THREADID
 */
bool SniperTraceReader::GetNextAccess( TraceLine *nextAccess )
{
    //printf("[NVMSIM][DEBUG] GetNextAccess(nextAccess: %10p)\n", (void *) nextAccess);

    /* We will read in a full line and fill in these values */
    unsigned int cycle = 0;
    OpType operation = READ;
    uint64_t address;
    NVMDataBlock dataBlock;
    NVMDataBlock oldDataBlock;
    unsigned int threadId = 0;
    
    int fd, response;
    fd = open(tracefile.c_str(), O_RDONLY);
    if(fd != -1)
    {
        std::string message_from_sniper;
        std::string message_to_sniper;
        char buffer[255];
        response = read(fd, buffer, 255);

        if(response)
        {
            /*
            *  Again, the format is : CYCLE OP ADDRESS DATA THREADID
            *  So the field ids are :   0    1    2      3      4
            */
            message_from_sniper = "";
            int index = 0;
            while (buffer[ index ] != '\n')
                message_from_sniper += buffer[ index++ ];

#ifdef NVMSIM_DEBUG
            printf("[NVMSIM][DEBUG][RCIVE] %s\n", message_from_sniper.c_str());
#endif
            std::istringstream lineStream( message_from_sniper );
            std::string field;
            unsigned char fieldId = 0;

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
                        {
                            return false;      
                        }
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
        }
        else
        {
            printf("[NVMSIM][ERROR][READ ] Error on read message\n");
            return false;      
        }
        close(fd); 
    }
    else
    {
        return false;
    }

    NVMAddress nAddress;
    nAddress.SetPhysicalAddress( address );

    nextAccess->SetLine( nAddress, operation, cycle, dataBlock, oldDataBlock, threadId );

    return true;
}

/* 
 * Get the next N accesses to main memory. Called GetNextAccess N times and 
 * places the return values into a vector of TraceLine pointers.
 */
int SniperTraceReader::GetNextNAccesses( unsigned int N, std::vector<TraceLine *> *nextAccesses )
{
    //printf("[NVMSIM][DEBUG] GetNextNAccesses(N: %d, nextAccesses: %p)\n", N, (void *) nextAccesses);
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

bool SniperTraceReader::Read() 
{
    return true;
}

bool SniperTraceReader::Write(uint64_t latency)
{
#ifdef NVMSIM_TRACE
   printf("[NVMSIM][TRACE][WRITE] latency: %" PRIu64 "\n", latency);
#endif
    int fd, response = 0;
    fd = open(tracefile.c_str(), O_WRONLY);
    if(fd != -1)
    {
        std::ostringstream ss_latency;
        ss_latency << latency;
        message_to_sniper = "";
        message_to_sniper += ss_latency.str();
#ifdef NVMSIM_DEBUG
        printf("[NVMSIM][DEBUG][SEND ] %s\n", message_to_sniper.c_str());
#endif
        message_to_sniper += "\n";

        response = write(fd, message_to_sniper.c_str(), message_to_sniper.length()); 
        if(!response)
        {
            printf("[NVMSIM][ERROR][WRITE] Error on writing message\n");
        }
        close(fd); 
    }
    else
    {
        return false;
    }
    return response;
}