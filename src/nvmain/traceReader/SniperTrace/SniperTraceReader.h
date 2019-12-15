#ifndef __SNIPERTRACEREADER_H__
#define __SNIPERTRACEREADER_H__

#include "traceReader/GenericTraceReader.h"
#include <string>
#include <iostream>
#include <fstream>

namespace NVM {

class SniperTraceReader : public GenericTraceReader
{
  public:
    SniperTraceReader( );
    ~SniperTraceReader( );
    
    void SetTraceFile( std::string file );
    std::string GetTraceFile( );
    
    bool GetNextAccess( TraceLine *nextAccess );
    int  GetNextNAccesses( unsigned int N, std::vector<TraceLine *> *nextAccess );
  
  private:
    std::string traceFile;
    std::ifstream trace;
    unsigned int traceVersion;
    bool readVersion;
};

};

#endif
