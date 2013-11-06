#ifndef _PHYSICSANALYZER_H
#define _PHYSICSANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"

namespace physical{

  class Physics_Analyzer:public RN_Analyzer{
  private:
    
  public:  
    Physics_Analyzer();
    virtual ~Physics_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual void Clear(){};
    virtual bool Terminate();
    virtual void ResetGlobals();
    
    
    ClassDef(Physics_Analyzer,0);
  };
  
}


#endif
