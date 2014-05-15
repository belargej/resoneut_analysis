/***************************************************************
//Class: Si_NEUT_IC_NAI_Analyzer
//
//Author:Sean Kuvin
//
//Si_NEUT_IC_Analyzer is added to the analyzer list just as S2_Analyzer
//is except now we are looking at Silicon detector parameters,Ion 
//Chamber parameters and Neutron Detector parameters.  
//The results of sorting through
//the Si_Analyzer,the IC_analyzer,S2_IC_Analyzer, and the NeutAnalyzer 
//are necessary for proper function of this class. 
//Therefore, all 4 analyzers must be 
//added to the analyzer list and they all must be
// added before this analyzer is added
********************************************************************/

#ifndef _Si_Neut_NAI_IC_ANALYZER_H
#define _Si_Neut_NAI_IC_ANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"

namespace coinc{

  class Si_Neut_IC_NaI_Analyzer:public RN_Analyzer{
  private:
    
  public:
    
    Si_Neut_IC_NaI_Analyzer();
    virtual ~Si_Neut_IC_NaI_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    virtual void Reset();
    
    
    ClassDef(Si_Neut_IC_NaI_Analyzer,0);

  };

  extern int sia_n_time_check;
  extern int sib_n_time_check;

  void Require_SIA_N_TimePeak();
  void Require_SIB_N_TimePeak();
  void Load_Si_Neut_IC_NaI_Gates(const std::string & input);

}


#endif
