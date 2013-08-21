#if defined (__CINT__)||defined (__MAKECINT__)
#define mEXTERN R__EXTERN
#else
#define mEXTERN extern
#endif

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


  mEXTERN TFile *rootfile;
  

  //declare Gates
  mEXTERN TCutG* n0_neuts;
  mEXTERN TCutG* n1_neuts;
  mEXTERN TCutG* n2_neuts;
  mEXTERN TCutG* n3_neuts;
  mEXTERN TCutG* n4_neuts;
  mEXTERN TCutG* n5_neuts;
  mEXTERN TCutG* n6_neuts;
  mEXTERN TCutG* n7_neuts;
  mEXTERN TCutG* n8_neuts;
  mEXTERN TCutG* n9_neuts;
  mEXTERN TCutG* prots1;
  mEXTERN TCutG* alphas;
  
  //declare histograms here
  
  mEXTERN sak::Histogram1D *hrftime;
  mEXTERN sak::Hist1D *hrftime_allneut;
  mEXTERN sak::Hist2D *hrftime_n;
  mEXTERN sak::Hist2D *hrftime_raw_n;
  mEXTERN sak::Hist2D *hrftime_gated_n;
  mEXTERN sak::Histogram1D *hrftime_cal;
  mEXTERN sak::Hist1D *hrftime_allneut_cal;
  mEXTERN sak::Hist1D *hrftime_allneut_cal_p;
  mEXTERN sak::Hist1D *hrftime_allneut_cal_proton;
  mEXTERN sak::Hist1D *hrftime_allneut_cal_alphas;
  mEXTERN sak::Hist1D *h_ndetMult;
  mEXTERN sak::Hist1D *h_ndetMult_ngated;
  mEXTERN sak::Hist1D *h_ndetMult_npgated;
  mEXTERN sak::Hist2D *s2_tvrf;
  mEXTERN sak::Hist2D *s2_tvrf_neut_gated;
  mEXTERN sak::Hist2D *s2_e_v_theta;
  mEXTERN sak::Hist2D *s2_e_v_theta_ngated;
  mEXTERN sak::Hist2D *s2_e_v_theta_protons;
  mEXTERN sak::Hist1D *hrftime_prots;
  mEXTERN sak::Hist2D *hPSD_n1_prots;
  mEXTERN sak::Histogram2D *hPSDq_n[10];
  mEXTERN sak::Histogram2D *hPSD_n_[10];
  mEXTERN sak::Histogram2D *hTrel_n[10];
  mEXTERN sak::Histogram2D *hEvT_n[10];
  mEXTERN sak::Hist2D* hpede;
  mEXTERN sak::Hist2D* hpede_2;
  mEXTERN sak::Hist2D* hpede_ngated;


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
