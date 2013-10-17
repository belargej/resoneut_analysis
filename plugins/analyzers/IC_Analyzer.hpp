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
    virtual bool Terminate();
    virtual void Clear();
    
    
    ClassDef(IC_Analyzer,0);
  };

  extern TCutG* ede_hi1;
  extern TCutG* ede_hi2;
  extern TCutG* ede_hi3;

  extern int hi_check[3];


  void LoadGates(const std::string &input);  
  void ClearGates();  

}


#endif
