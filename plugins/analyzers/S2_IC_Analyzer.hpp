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
    virtual bool Terminate();
    virtual void Clear();
    virtual void ResetGlobals();
    
    
    ClassDef(S2_IC_Analyzer,0);
  };


  extern int si_ic_tcheck;

  void Load_Si_IC_Gates(const std::string& input);
  
}


#endif
