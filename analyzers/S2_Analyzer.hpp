#ifndef _S2ANALYZER
#define _S2ANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "sak/RNHistograms.hpp"
#include "sak/RNGates.hpp"
#include "calibrator/RN_Calibrator.hpp"


class S2_Analyzer:public RN_Analyzer{
private:

public:
  TFile *rootfile;
  sak::Histogram2D *front[16];
 // sak::Histogram1D front0;
 // sak::Histogram1D front1;
 // sak::Histogram1D front2;
 // sak::Histogram1D front3;
 // sak::Histogram1D front4;
 // sak::Histogram1D front5;
 // sak::Histogram1D front6;
 // sak::Histogram1D front7;
 // sak::Histogram1D front8;
 // sak::Histogram1D front9;
 // sak::Histogram1D front10;
 // sak::Histogram1D front11;
 // sak::Histogram1D front12;
 // sak::Histogram1D front13;
 // sak::Histogram1D front14;
 // sak::Histogram1D front15;

  
  S2_Analyzer();
  virtual ~S2_Analyzer(){};
  void AutoCalibrate(int matchfront, int matchback);//auto calibrate all elements of Collection
 
  virtual void initHists(std::string);
  virtual void Loop();
  virtual void Process();
  virtual void WriteOut();
  virtual void LoadGates(const char *a);
  virtual void Clear(){};


  ClassDef(S2_Analyzer,0);
};


#endif
