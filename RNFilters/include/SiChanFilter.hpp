/***********************************************************/
//Class: SiChanFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
/***********************************************************/

#ifndef _SiChanFilter_H
#define _SiChanFilter_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class SiChanFilter:public RN_Analyzer{
  private:   
    bool KillBackEquFront;
    bool KillBackEquFrontPlusOne;
  public:
    
    SiChanFilter();
    virtual ~SiChanFilter(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();

    void SetKillBackEquFront(bool On = true){KillBackEquFront = On;};
    void SetKillBackEquFrontPlusOne(bool On = true){KillBackEquFrontPlusOne = On;};
    
    ClassDef(SiChanFilter,0);
  };
  
}


#endif
