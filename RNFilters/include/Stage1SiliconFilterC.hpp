/***********************************************************/
//Class: Stage1SiliconFilterC
//
//Author:Sean Kuvin
//
//FilterC code to extract data that comes with a valid Silicon Time
//a valid Ion chamber time, no ICds trigger bit, no overflow bits in the
//silicon channels, and a maximum silicon multiplicity of 3.
/***********************************************************/

#ifndef _Stage1_SiliconFilterC_H
#define _Stage1_SiliconFilterC_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace RNfilters{



  class Stage1SiliconFilterC:public RN_Analyzer{
  private:    
  public:
    
    Stage1SiliconFilterC();
    virtual ~Stage1SiliconFilterC(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    
    ClassDef(Stage1SiliconFilterC,0);
  };
  
}


#endif
