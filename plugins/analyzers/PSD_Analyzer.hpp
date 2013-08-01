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
  sak::Histogram2D *hPSD_n0;
  sak::Histogram2D *hPSD_n1;
  sak::Histogram2D *hPSD_n2;
  sak::Histogram2D *hPSD_n3;
  sak::Histogram2D *hPSD_n4;
  sak::Histogram2D *hPSD_n5;
  sak::Histogram2D *hPSD_n6;
  sak::Histogram2D *hPSD_n7;
  sak::Histogram2D *hPSD_n8;
  sak::Histogram2D *hPSD_n9;
  
  sak::Histogram2D *pEde;
  sak::Histogram2D *pEde_ngated;

  sak::Histogram2D *  hTrel_n0;
  sak::Histogram2D *  hTrel_n1;
  sak::Histogram2D *  hTrel_n2;
  sak::Histogram2D *  hTrel_n3;
  sak::Histogram2D *  hTrel_n4;
  sak::Histogram2D *  hTrel_n5;
  sak::Histogram2D *  hTrel_n6;
  sak::Histogram2D *  hTrel_n7;
  sak::Histogram2D *  hTrel_n8;
  sak::Histogram2D *  hTrel_n9;



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
