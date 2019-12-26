/*******************************************************************************
* Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
* Department of Computer Science and Engineering, The Pennsylvania State University
* All rights reserved.
* 
* This source code is part of NVMain - A cycle accurate timing, bit accurate
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

#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <fstream>

#include "src/Interconnect.h"
#include "Interconnect/InterconnectFactory.h"
#include "src/Config.h"
#include "src/TranslationMethod.h"
#include "traceReader/TraceReaderFactory.h"
#include "src/AddressTranslator.h"
#include "Decoders/DecoderFactory.h"
#include "src/MemoryController.h"
#include "MemControl/MemoryControllerFactory.h"
#include "Endurance/EnduranceDistributionFactory.h"
#include "SimInterface/NullInterface/NullInterface.h"
#include "include/NVMHelpers.h"
#include "Utils/HookFactory.h"
#include "src/EventQueue.h"
#include "NVM/nvmain.h"
#include "traceSim/traceMain.h"
#include "traceReader/SniperTrace/SniperTraceReader.h"

#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

#include <inttypes.h>

using namespace NVM;

int main( int argc, char *argv[] )
{
    printf("[NVMSIM] [traceMain.cpp] int main(...) <- ( argc, *argv[] )\n");
    printf("[NVMSIM] [traceMain.cpp] int main(...) <- ( %d, %p )\n", argc, (void *) argv);
    for(int i = 0; i < argc; i++)
        printf("[NVMSIM] [traceMain.cpp] int main(...) - argv[%d/%d]: %s\n", i+1, argc, argv[i]);
    printf("[NVMSIM] [traceMain.cpp] int main(...) - TraceMain *traceRunner = new TraceMain( )\n");
    TraceMain *traceRunner = new TraceMain( );
    printf("[NVMSIM] [traceMain.cpp] int main(...) - return traceRunner->RunTrace( argc, argv )\n");
    return traceRunner->RunTrace( argc, argv );
}

TraceMain::TraceMain( )
{
    printf("[NVMSIM] [traceMain.cpp] TraceMain( )\n");
}

TraceMain::~TraceMain( )
{
    printf("[NVMSIM] [traceMain.cpp] ~TraceMain( )\n");
}

int TraceMain::RunTrace( int argc, char *argv[] )
{
    printf("[NVMSIM] [traceMain.cpp] RunTrace(...) <- ( int argc, char *argv[] )\n");
    printf("[NVMSIM] [traceMain.cpp] RunTrace(...) <- ( %d, %p )\n", argc, (void *) argv);

    Stats *stats = new Stats( );
    Config *config = new Config( );
    GenericTraceReader *trace = NULL;
    TraceLine *tl = new TraceLine( );
    SimInterface *simInterface = new NullInterface( );
    NVMain *nvmain = new NVMain( );
    EventQueue *mainEventQueue = new EventQueue( );
    GlobalEventQueue *globalEventQueue = new GlobalEventQueue( );
    TagGenerator *tagGenerator = new TagGenerator( 1000 );
    bool IgnoreData = false;

    uint64_t simulateCycles;
    uint64_t currentCycle;
    
        
    std::string fifofile = "/tmp/nvmsim-fifofile";
    mkfifo(fifofile.c_str(), 0666);

    if( argc < 4 )
    {
        std::cout << "Usage: nvmain CONFIG_FILE TRACE_FILE CYCLES [PARAM=value ...]" << std::endl;
        return 1;
    }

    /* Print out the command line that was provided. */
    std::cout << "NVMain command line is:" << std::endl;
    for( int curArg = 0; curArg < argc; ++curArg )
    {
        std::cout << argv[curArg] << " ";
    }
    std::cout << std::endl << std::endl;

    config->Read( argv[1] );
    config->SetSimInterface( simInterface );
    SetEventQueue( mainEventQueue );
    SetGlobalEventQueue( globalEventQueue );
    SetStats( stats );
    SetTagGenerator( tagGenerator );
    std::ofstream statStream;

    /* Allow for overriding config parameter values for trace simulations from command line. */
    if( argc > 4 )
    {
        for( int curArg = 4; curArg < argc; ++curArg )
        {
            std::string clParam, clValue, clPair;
            
            clPair = argv[curArg];
            clParam = clPair.substr( 0, clPair.find_first_of("="));
            clValue = clPair.substr( clPair.find_first_of("=") + 1, std::string::npos );

            std::cout << "Overriding " << clParam << " with '" << clValue << "'" << std::endl;

            config->SetValue( clParam, clValue );
        }
    }

    if( config->KeyExists( "StatsFile" ) )
    {
        statStream.open( config->GetString( "StatsFile" ).c_str(), std::ofstream::out | std::ofstream::app );
    }

    if( config->KeyExists( "IgnoreData" ) && config->GetString( "IgnoreData" ) == "true" )
    {
        IgnoreData = true;
    }

    /*  Add any specified hooks */
    std::vector<std::string>& hookList = config->GetHooks( );

    for( size_t i = 0; i < hookList.size( ); i++ )
    {
        std::cout << "Creating hook " << hookList[i] << std::endl;

        NVMObject *hook = HookFactory::CreateHook( hookList[i] );
        
        if( hook != NULL )
        {
            AddHook( hook );
            hook->SetParent( this );
            hook->Init( config );
        }
        else
        {
            std::cout << "Warning: Could not create a hook named `" 
                << hookList[i] << "'." << std::endl;
        }
    }

    AddChild( nvmain );
    nvmain->SetParent( this );

    globalEventQueue->SetFrequency( config->GetEnergy( "CPUFreq" ) * 1000000.0 );
    globalEventQueue->AddSystem( nvmain, config );
    simInterface->SetConfig( config, true );
    nvmain->SetConfig( config, "defaultMemory", true );

    std::cout << "traceMain (" << (void*)(this) << ")" << std::endl;
    nvmain->PrintHierarchy( );
    
    if( config->KeyExists( "TraceReader" ) )
    {
        printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - trace is %s\n", config->GetString( "TraceReader" ).c_str());
        trace = TraceReaderFactory::CreateNewTraceReader( config->GetString( "TraceReader" ) );
    }
    else
    {
        printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - trace is NVMainTrace\n");
         trace = TraceReaderFactory::CreateNewTraceReader( "NVMainTrace" );
    }

    trace->SetTraceFile( argv[2] );

    if( argc == 3 )
        simulateCycles = 0;
    else
        simulateCycles = atoi( argv[3] );

    printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - simulateCycles: %" PRIu64 "\n", simulateCycles);
    std::cout << "*** Simulating " << simulateCycles << " input cycles. (";

    /*
     *  The trace cycle is assumed to be the rate that the CPU/LLC is issuing. 
     *  Scale the simulation cycles to be the number of *memory cycles* to run.
     */
    simulateCycles = (uint64_t)ceil( ((double)(config->GetValue( "CPUFreq" )) / (double)(config->GetValue( "CLK" ))) * simulateCycles ); 

    std::cout << simulateCycles << " memory cycles) ***" << std::endl;
    printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - simulateCycles: %" PRIu64 "\n", simulateCycles);

    currentCycle = 0;
    printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - currentCycle: %" PRIu64 "\n", simulateCycles);
    while( currentCycle <= simulateCycles || simulateCycles == 0 )
    {
        printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - currentCycle: %" PRIu64 "\n", simulateCycles);
        printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - trace->GetNextAccess( tl )\n");
        if( !trace->GetNextAccess( tl ) )
        {
            /* Force all modules to drain requests. */
            bool draining = Drain( );

            std::cout << "Could not read next line from trace file!" 
                << std::endl;

            /* Wait for requests to drain. */
            while( outstandingRequests > 0 )
            {
                globalEventQueue->Cycle( 1 );
              
                currentCycle++;

                /* Retry drain each cycle if it failed. */
                if( !draining )
                    draining = Drain( );
            }

            break;
        }

        NVMainRequest *request = new NVMainRequest( );
        
        request->address = tl->GetAddress( );
        request->type = tl->GetOperation( );
        request->bulkCmd = CMD_NOP;
        request->threadId = tl->GetThreadId( );
        if( !IgnoreData ) request->data = tl->GetData( );
        if( !IgnoreData ) request->oldData = tl->GetOldData( );
        request->status = MEM_REQUEST_INCOMPLETE;
        request->owner = (NVMObject *)this;

        /* 
         * If you want to ignore the cycles used in the trace file, just set
         * the cycle to 0. 
         */
        if( config->KeyExists( "IgnoreTraceCycle" ) 
                && config->GetString( "IgnoreTraceCycle" ) == "true" )
            tl->SetLine( tl->GetAddress( ), tl->GetOperation( ), 0, 
                         tl->GetData( ), tl->GetOldData( ), tl->GetThreadId( ) );

        if( request->type != READ && request->type != WRITE )
            std::cout << "traceMain: Unknown Operation: " << request->type 
                << std::endl;

        /* 
         * If the next operation occurs after the requested number of cycles,
         * we can quit. 
         */
        if( tl->GetCycle( ) > simulateCycles && simulateCycles != 0 )
        {
            globalEventQueue->Cycle( simulateCycles - currentCycle );
            currentCycle += simulateCycles - currentCycle;

            break;
        }
        else
        {
            /* 
             *  If the command is in the past, it can be issued. This would 
             *  occur since the trace was probably generated with an inaccurate 
             *  memory *  simulator, so the cycles may not match up. Otherwise, 
             *  we need to wait.
             */
            if( tl->GetCycle( ) > currentCycle )
            {
                globalEventQueue->Cycle( tl->GetCycle() - currentCycle );
                currentCycle = globalEventQueue->GetCurrentCycle( );

                if( currentCycle >= simulateCycles && simulateCycles != 0 )
                    break;
            }

            /* 
             *  Wait for the memory controller to accept the next command.. 
             *  the trace reader is "stalling" until then.
             */
            while( !GetChild( )->IsIssuable( request ) )
            {
                if( currentCycle >= simulateCycles && simulateCycles != 0 )
                    break;

                globalEventQueue->Cycle( 1 );
                currentCycle = globalEventQueue->GetCurrentCycle( );
            }

            outstandingRequests++;
            GetChild( )->IssueCommand( request );

            if( currentCycle >= simulateCycles && simulateCycles != 0 )
                break;
        }

        printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - opening fifofile to read...\n");
        int fd, response;
        fd = open(fifofile.c_str(), O_RDONLY);
        if(fd != -1)
        {
            printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - opening fifofile to read... done\n");
            
            std::string message_to_sniper;
            char message_from_sniper[255];

            response = read(fd, message_from_sniper, 255);
            if(response)
            {
                printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - {message_from_sniper: %s}\n", message_from_sniper);
            }
            else
            {
                printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - error on reading message\n");
            }
            close(fd); 
            
            printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - opening fifofile to write...\n");
            fd = open(fifofile.c_str(), O_WRONLY);
            if(fd != -1)
            {
                printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - opening fifofile to write... done\n");
                
                std::ostringstream ss_currentCycle;
                ss_currentCycle << currentCycle;
                message_to_sniper = "";
                message_to_sniper += ss_currentCycle.str();

                response = write(fd, message_to_sniper.c_str(), message_to_sniper.length()); 
                if(response)
                {
                    printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - {message_to_sniper: %s}\n", message_to_sniper.c_str());
                }
                else
                {
                    printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - error on writing message\n");
                }
                close(fd); 
            }
            else
            {
                printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - opening fifofile to write... error\n");
            }
            
        }
        else
        {
            printf("[NVMSIM] [traceMain.cpp] RunTrace(...) - opening fifofile to read... error\n");
        }

    }       

    GetChild( )->CalculateStats( );
    std::ostream& refStream = (statStream.is_open()) ? statStream : std::cout;
    stats->PrintAll( refStream );

    std::cout << "Exiting at cycle " << currentCycle << " because simCycles " << simulateCycles << " reached." << std::endl; 
    if( outstandingRequests > 0 )
        std::cout << "Note: " << outstandingRequests << " requests still in-flight." << std::endl;

    delete config;
    delete stats;

    return -3;
}

void TraceMain::Cycle( ncycle_t /*steps*/ )
{
    printf("[NVMSIM] [traceMain.cpp] Cycle( ncycle_t /*steps*/ )\n");
}

bool TraceMain::RequestComplete( NVMainRequest* request )
{
    printf("[NVMSIM] [traceMain.cpp] RequestComplete( NVMainRequest* request )\n");
    /* This is the top-level module, so there are no more parents to fallback. */
    assert( request->owner == this );

    outstandingRequests--;

    delete request;

    return true;
}
