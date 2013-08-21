#ifndef _PSDANALYZER
#define _PSDANALYZER
#include <TH2.h>
#include <TFile.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"

namespace psd{


  class NeutAnalyzer:public RN_Analyzer{
  private:
    
  public:
    
  NeutAnalyzer();
  virtual ~NeutAnalyzer(){};

  virtual void Begin();
  
  virtual void Process();
  virtual void Terminate();
  
  virtual void Clear(){};


  ClassDef(NeutAnalyzer,0);

};


  void analysis(const char* infile, const char* config);
  
  
}


#endif
