#ifndef _NTANALYZER
#define _NTANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "RNHistograms.cpp"
#include "RNGates.cpp"


class NewTree_Analyzer:public RN_Analyzer{
private:

public:
  TFile *rootfile;
  TTree *rawtree;
  TTree *caltree;
  Int_t calibrated;
  NewTree_Analyzer();
  virtual ~NewTree_Analyzer(){};

  void InitTree(std::string filename,Int_t calibrate=0);
  virtual void Loop();
  virtual void Process();
  virtual void Clear(){};
  virtual void WriteOut();

  ClassDef(NewTree_Analyzer,0);
};


#endif
