/***********************************************************/
//Class: NaIEnFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero NaI time.
/***********************************************************/

#ifndef _NaIEnFilter_H
#define _NaIEnFilter_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class NaIEnFilter:public RN_Analyzer{
  private:    
    int LowValue;
    int HighValue;
    bool UseGate;
    bool YesNaIEn;
  public:
    
    NaIEnFilter();
    virtual ~NaIEnFilter(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    void SetWindow(int Low, int High);
    void YesNaIEnOn(){YesNaIEn = true;UseGate = false;};
    ClassDef(NaIEnFilter,0);
  };
  
}


#endif
