#ifndef _PSDANALYZERTWO
#define _PSDANALYZERTWO
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


namespace psd{

  class NeutAnalyzer2:public RN_Analyzer{
  private:
    
  public:
    
    NeutAnalyzer2();
    virtual ~NeutAnalyzer2(){};
    
    virtual void Begin();
    
    virtual void Process();
    virtual void Terminate();
    virtual void LoadGates(const std::string&);
    virtual void Clear(){};
    
    
    ClassDef(NeutAnalyzer2,0);
  };
  


  void analysis2(const char * infile, const char * config, const char * cuts);
  
  
}


#endif
