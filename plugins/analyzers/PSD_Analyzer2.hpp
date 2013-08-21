#ifndef _PSDANALYZERTWO
#define _PSDANALYZERTWO
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "../plugins/sak/sak_Histograms.hpp"
#include "../plugins/sak/sak_Gates.hpp"


class PSD_Analyzer2:public RN_Analyzer{
private:

public:
  TFile *rootfile;

  //declare Gates
  TCutG* n0_neuts;
  TCutG* n1_neuts;
  TCutG* n2_neuts;
  TCutG* n3_neuts;
  TCutG* n4_neuts;
  TCutG* n5_neuts;
  TCutG* n6_neuts;
  TCutG* n7_neuts;
  TCutG* n8_neuts;
  TCutG* n9_neuts;
  TCutG* prots1;
  TCutG* alphas;

  //declare histograms here
  sak::Histogram1D *hrftime;
  sak::Hist1D *hrftime_allneut;
  sak::Hist2D *hrftime_n;
  sak::Hist2D *hrftime_raw_n;
  sak::Hist2D *hrftime_gated_n;
  sak::Histogram1D *hrftime_cal;
  sak::Hist1D *hrftime_allneut_cal;
  sak::Hist1D *hrftime_allneut_cal_p;
  sak::Hist1D *hrftime_allneut_cal_proton;
  sak::Hist1D *hrftime_allneut_cal_alphas;
  sak::Hist1D *h_ndetMult;
  sak::Hist1D *h_ndetMult_ngated;
  sak::Hist1D *h_ndetMult_npgated;
  sak::Hist2D *s2_tvrf;
  sak::Hist2D *s2_tvrf_neut_gated;
  sak::Hist2D *s2_e_v_theta;
  sak::Hist2D *s2_e_v_theta_ngated;
  sak::Hist2D *s2_e_v_theta_protons;
  sak::Hist1D *hrftime_prots;
  sak::Hist2D *hPSD_n1_prots;
  sak::Histogram2D *hPSDq_n[10];
  sak::Histogram2D *hPSD_n_[10];
  sak::Histogram2D *hTrel_n[10];
  sak::Histogram2D *hEvT_n[10];
  sak::Hist2D* hpede;
  sak::Hist2D* hpede_2;
  sak::Hist2D* hpede_ngated;

  //declare extra parameters here, these can be then retrieved properly by gates
  double prot_E;
  double prot_dE;
  double prot_theta;


  PSD_Analyzer2();
  virtual ~PSD_Analyzer2(){};

  virtual void Begin();

  virtual void Process();
  virtual void Terminate();
  virtual void LoadGates(const std::string&);
  virtual void Clear(){};


  ClassDef(PSD_Analyzer2,0);
};


namespace sak{
  void psd_analysis2(const char * infile, const char * config, const char * cuts);
  
  
}


#endif
