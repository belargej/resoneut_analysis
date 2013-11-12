/***************************************************************
//Class: S2_NEUT_IC_NAI_Analyzer
//
//Author:Sean Kuvin
//
//S2_NEUT_IC_Analyzer is added to the analyzer list just as S2_Analyzer
//is except now we are looking at Silicon detector parameters,Ion 
//Chamber parameters and Neutron Detector parameters.  
//The results of sorting through
//the S2_Analyzer,the IC_analyzer,S2_IC_Analyzer, and the NeutAnalyzer 
//are necessary for proper function of this class. 
//Therefore, all 4 analyzers must be 
//added to the analyzer list and they all must be
// added before this analyzer is added
********************************************************************/

#ifndef _S2_Neut_NAI_IC_ANALYZER_H
#define _S2_Neut_NAI_IC_ANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


namespace coinc{

  class S2_Neut_IC_NaI_Analyzer:public RN_Analyzer{
  private:
    
  public:
    
    S2_Neut_IC_NaI_Analyzer();
    virtual ~S2_Neut_IC_NaI_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    virtual void ResetGlobals();
    
    
    ClassDef(S2_Neut_IC_NaI_Analyzer,0);
  };


}


#endif
