
/***********************************************************/
//Class: IC_Analyzer
//
//Author:Sean Kuvin
//
//IC Analyzer class to be inserted in the analyzer list for 
//sorting Ion Chamber parameters. Making Histograms, 
//Loading Cuts, and checking validity of cuts should 
//all be performed here when considering Ion chamber 
//parameters on their own. 
/***********************************************************/



#ifndef _ICANALYZER_H
#define _ICANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


namespace ionchamber{

  class IC_Analyzer:public RN_Analyzer{
  private:
    
  public:
    
    IC_Analyzer();
    virtual ~IC_Analyzer(){};
    
    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual void ResetGlobals();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Clear();
    
    
    ClassDef(IC_Analyzer,0);
  };

  extern TCutG* ede_hi1;
  extern TCutG* ede_hi2;
  extern TCutG* ede_hi3;

  extern int hi_check[3];
  extern Double32_t IC_TotalE;
  extern Double32_t IC_ELoss;

  void LoadGates(const std::string &input);  
  void ClearGates();  

}


#endif
