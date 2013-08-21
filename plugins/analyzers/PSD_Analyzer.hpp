#if defined (__CINT__)||defined (__MAKECINT__)
#define mEXTERN R__EXTERN
#else
#define mEXTERN extern
#endif


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

  mEXTERN TFile* rootfile;
  
  //declare histograms here
  mEXTERN sak::Histogram1D *hrftime;
  mEXTERN sak::Hist2D *hrftime_n;
  mEXTERN sak::Histogram1D *hrftime_cal;
  mEXTERN sak::Hist1D *h_ndetMult;
  mEXTERN sak::Hist1D *h_ndetMult_ngated;
  mEXTERN sak::Hist1D *h_ndetMult_npgated;
  mEXTERN sak::Hist2D *s2_tvrf;
  mEXTERN sak::Hist2D *s2_e_v_theta;
  mEXTERN sak::Histogram2D *hPSDq_n[10];
  mEXTERN sak::Histogram2D *hPSD_n_[10];
  mEXTERN sak::Histogram2D *  hTrel_n[10];
  mEXTERN sak::Hist2D* hpede;
  mEXTERN sak::Hist2D* hpede_2;
    

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
