#ifndef __NVMAIN_SIMINTERFACE_SNIPER_H__
#define __NVMAIN_SIMINTERFACE_SNIPER_H__

#include "src/SimInterface.h"

namespace NVM {

    class SniperInterface : public SimInterface
    {
    public:
        SniperInterface( );
        ~SniperInterface( );

        unsigned int GetInstructionCount( int core );
        unsigned int GetCacheMisses( int core, int level );
        unsigned int GetCacheHits( int core, int level );
        unsigned int GetUserMisses( int core );

        bool HasInstructionCount( );
        bool HasCacheMisses( );
        bool HasCacheHits( );

        int  GetDataAtAddress( uint64_t address, NVMDataBlock *data );
        void SetDataAtAddress( uint64_t address, NVMDataBlock& data );
    };

};

#endif
