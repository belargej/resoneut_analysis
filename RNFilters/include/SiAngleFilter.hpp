/***********************************************************/
//Class: SiAngleFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
/***********************************************************/

#ifndef _SiAngleFilter_H
#define _SiAngleFilter_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class SiAngleFilter:public RN_Analyzer{
  private:    
    int LowValue;
    int HighValue;
    bool UseGate;
  public:
    
    SiAngleFilter();
    virtual ~SiAngleFilter(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    void SetWindow(int Low, int High);
    ClassDef(SiAngleFilter,0);
  };
  
}


#endif
