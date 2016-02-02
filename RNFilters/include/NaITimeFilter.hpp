/***********************************************************/
//Class: NaITimeFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero NaI time.
/***********************************************************/

#ifndef _NaITimeFilter_H
#define _NaITimeFilter_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class NaITimeFilter:public RN_Analyzer{
  private:    
    int LowValue;
    int HighValue;
    bool UseGate;
    bool YesNaITime;
  public:
    
    NaITimeFilter();
    virtual ~NaITimeFilter(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    void SetWindow(int Low, int High);
    void YesNaITimeOn(){YesNaITime = true;UseGate = false;};
    ClassDef(NaITimeFilter,0);
  };
  
}


#endif
