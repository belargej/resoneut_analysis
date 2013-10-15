#ifndef _PSDANALYZER
#define _PSDANALYZER
#include <TH2.h>
#include <TFile.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"
#include "../../include/RN_Root.hpp"

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
    virtual void ResetGlobals();


  ClassDef(NeutAnalyzer,0);

};
  
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

  extern TCutG* n0_gammas;
  extern TCutG* n1_gammas;
  extern TCutG* n2_gammas;
  extern TCutG* n3_gammas;
  extern TCutG* n4_gammas;
  extern TCutG* n5_gammas;
  extern TCutG* n6_gammas;
  extern TCutG* n7_gammas;
  extern TCutG* n8_gammas;
  extern TCutG* n9_gammas;

  extern TCutG* n0_neuts_raw;
  extern TCutG* n1_neuts_raw;
  extern TCutG* n2_neuts_raw;
  extern TCutG* n3_neuts_raw;
  extern TCutG* n4_neuts_raw;
  extern TCutG* n5_neuts_raw;
  extern TCutG* n6_neuts_raw;
  extern TCutG* n7_neuts_raw;
  extern TCutG* n8_neuts_raw;
  extern TCutG* n9_neuts_raw;

  extern TCutG* n0_gammas_raw;
  extern TCutG* n1_gammas_raw;
  extern TCutG* n2_gammas_raw;
  extern TCutG* n3_gammas_raw;
  extern TCutG* n4_gammas_raw;
  extern TCutG* n5_gammas_raw;
  extern TCutG* n6_gammas_raw;
  extern TCutG* n7_gammas_raw;
  extern TCutG* n8_gammas_raw;
  extern TCutG* n9_gammas_raw;

  extern int neutcheck[NEUTNUM];
  extern int neut_sansgamma[NEUTNUM];
  extern int rawneutcheck[NEUTNUM];
  extern int rawneut_sansrawgamma[NEUTNUM];
  extern int gammacheck[NEUTNUM];
  extern int rawgammacheck[NEUTNUM];
  extern int neut_orcheck;
  extern int rawneut_orcheck;
  extern int neut_sansgamma_orcheck;
  extern int rawneut_sansrawgamma_orcheck;
  


 
  void LoadGates(const std::string&);
  void ClearGates();


  
  
}


#endif
