#ifndef _NAINEUTANALYZERTWO
#define _NAINEUTANALYZERTWO
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "PSD_Analyzer2.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


namespace psd{

  class NaI_NeutAnalyzer:public RN_Analyzer{
  private:
    
  public:
    
    NaI_NeutAnalyzer();
    virtual ~NaI_NeutAnalyzer(){};
    
    virtual void Begin();
    
    virtual void Process();
    virtual void Terminate();
    
    virtual void Clear(){};
    
    
    ClassDef(NaI_NeutAnalyzer,0);
  };
  


  void nai_neut_analysis(const char * infile, const char * config, const char * cuts);
  
  
}


#endif
