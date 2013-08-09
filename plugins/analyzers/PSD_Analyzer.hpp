#ifndef _PSDANALYZER
#define _PSDANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


class PSD_Analyzer:public RN_Analyzer{
private:

public:
  TFile *rootfile;

  //declare Gates
  sak::Gate* neuts_n0;
  sak::Gate* neuts_n1;
  sak::Gate* neuts_n2;
  sak::Gate* neuts_n3;
  sak::Gate* neuts_n4;
  sak::Gate* neuts_n5;
  sak::Gate* neuts_n6;
  sak::Gate* neuts_n7;
  sak::Gate* neuts_n8;
  sak::Gate* neuts_n9;
  sak::GateList allneuts;

  //declare histograms here
  sak::Histogram1D *hrftime;
  sak::Hist1D *hrftime_allneut;
  sak::Hist2D *hrftime_n;
  sak::Hist2D *hrftime_gated_n;
  sak::Histogram1D *hrftime_cal;
  sak::Hist1D *hrftime_allneut_cal;
  sak::Histogram2D *hPSDq_n[10];
  sak::Histogram2D *hPSD_n_[10];
  sak::Histogram2D *  hTrel_n[10];
  

  //declare extra parameters here, these can be then retrieved properly by gates
  double prot_E;



  PSD_Analyzer();
  virtual ~PSD_Analyzer(){};

  virtual void Begin();
  //virtual void Loop();
  virtual void Process();
  virtual void Terminate();
  virtual void LoadGates(const std::string&);
  virtual void Clear(){};


  ClassDef(PSD_Analyzer,0);
};


#endif
