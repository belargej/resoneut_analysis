/*************************************************************/
//Class: Physics_Analyzer
//
//Author:Sean Kuvin
//
//Physics Analyzer class to be inserted in the analyzer list for 
//calculating physical parameters using the detector parameters
/**************************************************************/

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
    virtual bool ProcessFill();
    virtual void Clear(){};
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void ResetGlobals();
    
    
    ClassDef(Physics_Analyzer,0);
  };
  
}


#endif
