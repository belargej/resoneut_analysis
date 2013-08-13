#ifndef _S2ANALYZER
#define _S2ANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"
#include "RN_Calibrator.hpp"


class S2_Analyzer:public RN_Analyzer{
private:
  int ind_;

public:
  TFile *rootfile;
  sak::Histogram2D *front[16];
  
  S2_Analyzer();
  virtual ~S2_Analyzer(){};
  void AutoCalibrate(int matchfront, int matchback);//auto calibrate all elements of Collection
 
  virtual void initHists(std::string,int ind=0);
  virtual void Loop();
  virtual void Process();
  virtual void WriteOut();
  virtual void LoadGates(const char *a);
  virtual void Clear(){};


  ClassDef(S2_Analyzer,0);
};


#endif
