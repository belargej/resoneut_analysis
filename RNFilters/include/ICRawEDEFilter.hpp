/***********************************************************/
//Class: ICRawEDEFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
/***********************************************************/

#ifndef _ICRawEDEFilter_H
#define _ICRawEDEFilter_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class ICRawEDEFilter:public RN_Analyzer{
  private:    
    int LowValue;
    int HighValue;
    bool UseGate;
  public:
    
    ICRawEDEFilter();
    virtual ~ICRawEDEFilter(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    void SetWindow(int Low, int High);
    ClassDef(ICRawEDEFilter,0);
  };
  
}


#endif
