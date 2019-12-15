#include "traceReader/SniperTrace/SniperTraceReader.h"
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <arpa/inet.h>

using namespace NVM;

SniperTraceReader::SniperTraceReader( )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] SniperTraceReader( )\n");
    traceFile = "";

    traceVersion = 0;
    readVersion = false;
}

SniperTraceReader::~SniperTraceReader( )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] ~SniperTraceReader( )\n");
    if( trace.is_open( ) )
        trace.close( );
}

void SniperTraceReader::SetTraceFile( std::string file )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] SetTraceFile( std::string file )\n");
    printf("[NVMSIM] [SniperTraceReader.cpp] SetTraceFile( std::string file ) - {traceFile: %s}\n", file.c_str());
    traceFile = file;
}

std::string SniperTraceReader::GetTraceFile( )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] GetTraceFile( )\n");
    return traceFile;
}

/*
 *  This trace is printed from nvmain.cpp. The format is:
 *
 *  CYCLE OP ADDRESS DATA THREADID
 */
bool SniperTraceReader::GetNextAccess( TraceLine *nextAccess )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] GetNextAccess(...)\n");
    printf("[NVMSIM] [SniperTraceReader.cpp] GetNextAccess(...) - (...) -> ( TraceLine *nextAccess )\n");
    /* If there is no trace file, we can't do anything. */
    if( traceFile == "" )
    {
        std::cerr << "No trace file specified!" << std::endl;
        printf("[NVMSIM] [SniperTraceReader.cpp] GetNextAccess( TraceLine *nextAccess ) - No trace file specified! - return false\n");
        return false;
    }

    /* If the trace file is not open, open it if possible. */
    if( !trace.is_open( ) )
    {
        trace.open( traceFile.c_str( ) );
        if( !trace.is_open( ) )
        {
            std::cerr << "Could not open trace file: " << traceFile << "!" << std::endl;
            printf("[NVMSIM] [SniperTraceReader.cpp] GetNextAccess( TraceLine *nextAccess ) - Could not open trace file - return false\n");
            return false;
        }
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
        printf("[NVMSIM] [SniperTraceReader.cpp] GetNextAccess( TraceLine *nextAccess ) - Reached EOF! - return false\n");
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
        std::cout << "SniperTraceReader: Unknown Operation: " << operation << "Line number is " << linenum << ". Full Line is \"" << fullLine << "\"" << std::endl;

    /*
     *  Set the line parameters.
     */
    NVMAddress nAddress;

    nAddress.SetPhysicalAddress( address );
    printf("[NVMSIM] [SniperTraceReader.cpp] GetNextAccess(...) - linenum: %d\n", linenum);
    nextAccess->SetLine( nAddress, operation, cycle, dataBlock, oldDataBlock, threadId );

    printf("[NVMSIM] [SniperTraceReader.cpp] GetNextAccess(...) - return true\n");
    return true;
}

/* 
 * Get the next N accesses to main memory. Called GetNextAccess N times and 
 * places the return values into a vector of TraceLine pointers.
 */
int SniperTraceReader::GetNextNAccesses( unsigned int N, std::vector<TraceLine *> *nextAccesses )
{
    printf("[NVMSIM] [SniperTraceReader.cpp] GetNextNAccesses( unsigned int N, std::vector<TraceLine *> *nextAccesses )\n");
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
