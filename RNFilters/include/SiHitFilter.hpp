/***********************************************************/
//Class: SiHitFilter
//
//Author:Belarge
//
// Filter events that to not have a nonzero IC time.
/***********************************************************/

#ifndef _SiHitFilter_H
#define _SiHitFilter_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class SiHitFilter:public RN_Analyzer{
  private:    
    bool UseSingleHit;
    bool UseDoubleHit;
    double SiAThresh;
    double SiBThresh;
  public:
    
    SiHitFilter();
    virtual ~SiHitFilter(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    void SetSingleHit(bool On = true){UseSingleHit = On; UseDoubleHit = false;};
    void SetDoubleHit(bool On = true){UseDoubleHit = On; UseSingleHit = false;};
    void SetSiAThresh(double Thresh = 0){SiAThresh = Thresh;};
    void SetSiBThresh(double Thresh = 0){SiBThresh = Thresh;};
    ClassDef(SiHitFilter,0);
  };
  
}


#endif
