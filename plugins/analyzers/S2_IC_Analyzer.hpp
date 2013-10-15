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
    
    virtual void Begin();
    virtual void Process();
    virtual void Terminate();
    virtual void Clear();
    virtual void ResetGlobals();
    
    
    ClassDef(S2_IC_Analyzer,0);
  };


}


#endif
