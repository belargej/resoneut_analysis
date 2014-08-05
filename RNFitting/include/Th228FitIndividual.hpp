/***********************************************************/
//Class: Th228FitIndividual
//
//Author:Sean Kuvin
//
//
//This class calculates any angular dependence on the phi segment energy
//The dependence is used to determine the dead layer thickness for the detector.
//The only input that is necessary is a root file which contains data
//from a Th228 alpha source. 
/***********************************************************/

#ifndef _TH228FITIND_H
#define _TH228FITIND_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Calibrator.hpp"

namespace si_cal{



  class Th228FitIndividual:public RN_Analyzer{
  private:    
  public:
    
    Th228FitIndividual();
    virtual ~Th228FitIndividual(){};
    
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual void Clear();
    virtual void Reset();
    
    
    ClassDef(Th228FitIndividual,0);
  };
  
}


#endif
