#ifndef _PSDANALYZER
#define _PSDANALYZER
#include <TH2.h>
#include <TFile.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
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

  extern TFile* rootfile;
  
  //declare Gates
  extern TCutG* n0_neuts;
  extern TCutG* n1_neuts;
  extern TCutG* n2_neuts;
  extern TCutG* n3_neuts;
  extern TCutG* n4_neuts;
  extern TCutG* n5_neuts;
  extern TCutG* n6_neuts;
  extern TCutG* n7_neuts;
  extern TCutG* n8_neuts;
  extern TCutG* n9_neuts;
  extern TCutG* prots1;
  extern TCutG* prots2;
  extern TCutG* alphas;
  extern TCutG* s1_delayed_rf;
  extern TCutG* s2_delayed_rf;
  extern TCutG* n0_evt;
  extern TCutG* n1_evt;
  extern TCutG* n2_evt;
  extern TCutG* n3_evt;
  extern TCutG* n4_evt;
  extern TCutG* n5_evt;
  extern TCutG* n6_evt;
  extern TCutG* n7_evt;
  extern TCutG* n8_evt;
  extern TCutG* n9_evt;







  void SetEvtGates();
  void LoadGates(const std::string&);



  
  
}


#endif
