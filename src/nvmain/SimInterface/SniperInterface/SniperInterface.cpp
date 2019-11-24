#include "SimInterface/SniperInterface/SniperInterface.h"
#include "base/statistics.hh"
#include "base/types.hh"
#include "base/stats/info.hh"
#include "sim/core.hh"
#include "sim/stat_control.hh"
#include "mem/ruby/system/RubySystem.hh"
#include "mem/ruby/profiler/Profiler.hh"
#include "mem/ruby/network/Network.hh"
#include "mem/ruby/network/Topology.hh"
#include <sstream>
#include <list>
#include <stdlib.h>

using namespace NVM;

SniperInterface::SniperInterface( )
{
}

SniperInterface::~SniperInterface( )
{
}

unsigned int SniperInterface::GetInstructionCount( int core )
{
   std::stringstream fmt, fmt2;
   unsigned int rv = 0;

   fmt.str("");
   fmt2.str("");

   /* Valid for timing simple CPU... need to check O3 etc. */
   if( core == -1 )
   {
      fmt << "sim_insts";
   }
   else
   {
      fmt << "system.cpu" << core << ".committedInsts";
      fmt2 << "system.cpu" << core << ".commit.committedInsts";
   }

   std::list<Stats::Info *> &allStats = Stats::statsList();
   std::list<Stats::Info *>::iterator it;

   for( it = allStats.begin(); it != allStats.end(); it++ )
   {
      Stats::Info* curStat = (*it);

      if( curStat->name == fmt.str()
          || ( core == 0 && curStat->name == "system.cpu.committedInsts" ) )
      {
         Stats::ScalarInfo *scalar = (Stats::ScalarInfo *)curStat;

         rv = (unsigned int)scalar->total();
      }
   }

   return rv;
}

unsigned int SniperInterface::GetCacheMisses( int core, int level )
{
#ifdef RUBY
   std::vector<AbstractController*> cntrls =
        g_system_ptr->getNetwork()->getTopologyPtr()->getControllerVector();

    std::stringstream fmt, fmt2;
    unsigned int rv = 0;

    fmt.str("");
    fmt << "L" << level << "Cache";

    if( level > 1 )
    {
        for( size_t i = 0; i < cntrls.size(); i++ )
        {
            if( cntrls[i]->getName() == fmt.str() )
            {
                rv += static_cast<unsigned int>(cntrls[i]->getCacheProfiler()->getMisses());
            }
        }
    }
    else if( level == 1 )
    {
        unsigned int L1count = 0;

        for( size_t i = 0; i < cntrls.size(); i++ )
        {
            if( cntrls[i]->getName() == fmt.str() )
            {
                if( L1count == core )
                {
                    rv = static_cast<unsigned int>(cntrls[i]->getCacheProfiler()->getMisses());
                    break;
                }
                else
                {
                    L1count++;
                }
            }
        }
    }
    /* I'm going to call the cpu level 0. Deal with it. */
    else if( level == 0 )
    {
        fmt.str("");

        fmt << "system.cpu" << core << "num_mem_refs";
        fmt2 << "system.cpu.num_mem_refs";

        std::list<Stats::Info *> &allStats = Stats::statsList();
        std::list<Stats::Info *>::iterator it;

        for( it = allStats.begin(); it != allStats.end(); it++ )
        {
            Stats::Info* curStat = (*it);

            if( curStat->name == fmt.str() || curStat->name == fmt2.str() )
            {
                Stats::ScalarInfo *scalar = (Stats::ScalarInfo *)curStat;

                rv = (unsigned int)scalar->total();
            }
        }
    }

    return rv;

#else
   std::stringstream fmt, fmt2;
   unsigned int rv = 0;

   fmt.str("");
   fmt2.str("");

   /* Valid for timing simple CPU... need to check O3 etc. */
   if( level > 1 )
   {
      fmt << "system.l" << core << ".overall_misses";
   }
   else if( level == 1 )
   {
      fmt << "system.cpu.dcache.overal_misses";
      fmt2 << "system.cpu" << core << ".dcache.overall_misses";
   }
   else if( level == 0 )
   {
      fmt.str("");

      fmt << "system.cpu" << core << "num_mem_refs";
      fmt2 << "system.cpu.num_mem_refs";

      std::list<Stats::Info *> &allStats = Stats::statsList();
      std::list<Stats::Info *>::iterator it;

      for( it = allStats.begin(); it != allStats.end(); it++ )
      {
         Stats::Info* curStat = (*it);

         if( curStat->name == fmt.str() || curStat->name == fmt2.str() )
         {
            Stats::ScalarInfo *scalar = (Stats::ScalarInfo *)curStat;

            rv = (unsigned int)scalar->total();
         }
      }
   }

   std::list<Stats::Info *> &allStats = Stats::statsList();
   std::list<Stats::Info *>::iterator it;

   for( it = allStats.begin(); it != allStats.end(); it++ )
   {
      Stats::Info* curStat = (*it);

      if( curStat->name == fmt.str() || curStat->name == fmt2.str() )
      {
         Stats::ScalarInfo *scalar = (Stats::ScalarInfo *)curStat;

         rv = (unsigned int)scalar->total();
      }
   }

   return rv;
#endif
}

unsigned int SniperInterface::GetCacheHits( int core, int level )
{
   if( level == 0 )
      return 0;

   return ( GetCacheMisses( core, level-1 ) - GetCacheMisses( core, level ) );
}

unsigned int SniperInterface::GetUserMisses( int core )
{
   /* No current way to differentiate user/supervisor accesses */
   return 0;
}

bool SniperInterface::HasInstructionCount( )
{
   return true;
}

bool SniperInterface::HasCacheMisses( )
{
   return true;
}

bool SniperInterface::HasCacheHits( )
{
   return true;
}

int  SniperInterface::GetDataAtAddress( uint64_t /*address*/, NVMDataBlock* /*data*/ )
{
   /* gem5 sends previous and new data, so this is not needed. */
   return 0;
}

void SniperInterface::SetDataAtAddress( uint64_t /*address*/, NVMDataBlock& /*data*/ )
{
   /* gem5 sends previous and new data, so this is not needed. */
   return;
}

