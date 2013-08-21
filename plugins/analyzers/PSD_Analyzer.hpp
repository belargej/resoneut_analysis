#ifndef _PSDANALYZER
#define _PSDANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"



class PSD_Analyzer:public RN_Analyzer{
private:

public:
  TFile *rootfile;

  //declare histograms here
  sak::Histogram1D *hrftime;
  sak::Hist2D *hrftime_n;
  sak::Histogram1D *hrftime_cal;
  sak::Hist1D *h_ndetMult;
  sak::Hist1D *h_ndetMult_ngated;
  sak::Hist1D *h_ndetMult_npgated;
  sak::Hist2D *s2_tvrf;
  sak::Hist2D *s2_e_v_theta;
  sak::Histogram2D *hPSDq_n[10];
  sak::Histogram2D *hPSD_n_[10];
  sak::Histogram2D *  hTrel_n[10];
  sak::Hist2D* hpede;
  sak::Hist2D* hpede_2;

  //declare extra parameters here, these can be then retrieved properly by gates
  double prot_E;
  double prot_dE;
  double prot_theta;


  PSD_Analyzer();
  virtual ~PSD_Analyzer(){};

  virtual void Begin();
  
  virtual void Process();
  virtual void Terminate();
  
  virtual void Clear(){};


  ClassDef(PSD_Analyzer,0);

};

namespace sak{
  void psd_analysis(const char* infile, const char* config);


}


#endif
