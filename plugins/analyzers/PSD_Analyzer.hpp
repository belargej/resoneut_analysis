#ifndef _PSDANALYZER
#define _PSDANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RNHistograms.hpp"
#include "RNGates.hpp"


class PSD_Analyzer:public RN_Analyzer{
private:

public:
  TFile *rootfile;

  //declare histograms here
  sak::Histogram1D *hrftime;
  sak::Hist1D *hrftime_allneut;
  sak::Histogram1D *hrftime_cal;
  sak::Hist1D *hrftime_allneut_cal;
  sak::Hist1D *hrftime_allneut_cal_p;
  sak::Histogram2D *hPSDq_n[10];

  
  sak::Histogram2D *hPSD_n_[10];

  sak::Histogram2D *pEde;
  sak::Histogram2D *pEde_ngated;

  sak::Histogram2D *  hTrel_n[10];
  

  //declare extra parameters here, these can be then retrieved properly by gates
  double prot_E;



  PSD_Analyzer();
  virtual ~PSD_Analyzer(){};

  virtual void initHists(std::string);
  virtual void Loop();
  virtual void Process();
  virtual void WriteOut();
  virtual void LoadGates(const char *a);
  virtual void Clear(){};


  ClassDef(PSD_Analyzer,0);
};


#endif
