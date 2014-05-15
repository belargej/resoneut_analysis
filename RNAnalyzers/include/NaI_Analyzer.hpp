/***************************************************************/
//Class: NaI_Analyzer
//
//Author:Sean Kuvin
//
//NaI_Analyzer is added to the analyzer list to sort NaI parameters
/********************************************************************/


#ifndef _NAI_ANALYZER_H
#define _NAI_ANALYZER_H
#include <TH2.h>
#include <TFile.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <new>
#include "RN_Analyzer.hpp"
#include "RN_Root.hpp"

namespace gamma_nai{

  class NaI_Analyzer:public RN_Analyzer{
  private:
  
  public:
  
    NaI_Analyzer();
    virtual ~NaI_Analyzer(){};
  
    virtual bool Begin();
  
    virtual bool Process();
    virtual bool ProcessFill();
    
    virtual bool Terminate();
    virtual bool TerminateIfLast();

    virtual void Clear(){};
    virtual void Reset();

  
    ClassDef(NaI_Analyzer,0);
  

  };

  
  extern Double32_t nai_t;
 


}

#endif
