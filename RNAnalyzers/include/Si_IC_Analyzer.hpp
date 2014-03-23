/***************************************************************/
//Class: Si_IC_Analyzer
//
//Author:Sean Kuvin
//
//Si_IC_Analyzer is added to the analyzer list just as S2_Analyzer
//is except now we are looking at Silicon detector parameters
//alongside Ion Chamber parameters.  The results of sorting through
//the S2_Analyzer and the IC_analyzer are necessary for proper function
//of this class. Therefore, both S2_Analyzer and IC_Analyzer must be 
//added to the analyzer list and they both must be added before this analyzer
//is added
/********************************************************************/


#ifndef _Si_IC_ANALYZER
#define _Si_IC_ANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"




namespace coinc{

  class Si_IC_Analyzer:public RN_Analyzer{
  private:
    
  public:
    
    Si_IC_Analyzer();
    virtual ~Si_IC_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    virtual void ResetGlobals();
    
    
    ClassDef(Si_IC_Analyzer,0);
  };


  extern int si_ic_tcheck;

  void Require_Si_IC_Time();

  void Load_Si_IC_Gates(const std::string& input);
  
}


#endif
