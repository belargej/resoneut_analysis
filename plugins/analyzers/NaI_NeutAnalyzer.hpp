/***************************************************************/
//Class: NaI_Neut_Analyzer
//
//Author:Sean Kuvin
//
//NaI_Neut_Analyzer is added to the analyzer list just as NaI_Analyzer
//is except now we are looking at Silicon detector parameters
//alongside Ion Chamber parameters.  The results of sorting through
//the Neut_analyzer is necessary for proper function
//of this class. Therefore, Neut_Analyzer must be 
//added to the analyzer list and must be added before this analyzer
//is added
/********************************************************************/


#ifndef _NAI_NEUTANALYZER_H
#define _NAI_NEUTANALYZER_H
#include <TH2.h>
#include <TFile.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"
#include "../../include/RN_Root.hpp"

namespace coinc{

  class NaI_NeutAnalyzer:public RN_Analyzer{
  private:
  
  public:
  
    NaI_NeutAnalyzer();
    virtual ~NaI_NeutAnalyzer(){};
  
    virtual bool Begin();
  
    virtual bool Process();
    virtual bool ProcessFill();
    
    virtual bool Terminate();
    virtual bool TerminateIfLast();

    virtual void Clear(){};
    virtual void ResetGlobals();

  
    ClassDef(NaI_NeutAnalyzer,0);
  

  };

  
  extern Double32_t nai_t;
  extern Double32_t neut_t;


}

#endif
