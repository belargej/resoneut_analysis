/***************************************************************/
//Class: S2_IC_Analyzer
//
//Author:Sean Kuvin
//
//S2_IC_Analyzer is added to the analyzer list just as S2_Analyzer
//is except now we are looking at Silicon detector parameters
//alongside Ion Chamber parameters.  The results of sorting through
//the S2_Analyzer and the IC_analyzer are necessary for proper function
//of this class. Therefore, both S2_Analyzer and IC_Analyzer must be 
//added to the analyzer list and they both must be added before this analyzer
//is added
/********************************************************************/


#ifndef _S2_IC_ANALYZER
#define _S2_IC_ANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


namespace coinc{

  class S2_IC_Analyzer:public RN_Analyzer{
  private:
    
  public:
    
    S2_IC_Analyzer();
    virtual ~S2_IC_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    virtual void ResetGlobals();
    
    
    ClassDef(S2_IC_Analyzer,0);
  };


  extern int si_ic_tcheck;

  void Load_Si_IC_Gates(const std::string& input);
  
}


#endif
