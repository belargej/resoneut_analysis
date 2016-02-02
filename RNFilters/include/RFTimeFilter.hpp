/***********************************************************/
//Class: RFTimeFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero RF time.
/***********************************************************/

#ifndef _RFTimeFilter_H
#define _RFTimeFilter_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class RFTimeFilter:public RN_Analyzer{
  private:    
    int LowValue1;
    int HighValue1;
    int LowValue2;
    int HighValue2;  
    int LowValue3;
    int HighValue3;
    int LowValue4;
    int HighValue4;
    bool UseRawValue;
    bool UseSiSubtract;
  public:
    
    RFTimeFilter();
    virtual ~RFTimeFilter(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();

    void SetWindow(int Low, int High);
    void SetWindow(int Low1, int High1, int Low2, int High2);
    void SetWindow(int Low1, int High1, int Low2, int High2, int Low3, int High3);
    void SetWindow(int Low1, int High1, int Low2, int High2, int Low3, int High3, int Low4, int High4);
    void SetUseRaw(bool UseRaw  = 1);
    void SetUseSiSubtract(bool UseRaw = 1){UseSiSubtract = UseRaw;};
    
    ClassDef(RFTimeFilter,0);
  };
  
}


#endif
