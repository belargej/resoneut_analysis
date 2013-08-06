#ifndef _NTANALYZER
#define _NTANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "analyzers/RN_Analyzer.hpp"
#include "sak/RNHistograms.hpp"
#include "sak/RNGates.hpp"


class NewTree_Analyzer:public RN_Analyzer{
private:

public:
  TFile *rootfile;
  TTree *tree;
  
  NewTree_Analyzer();
  virtual ~NewTree_Analyzer(){};

  void InitTree(std::string filename,std::string treename);
  virtual void Loop();
  virtual void Process();
  virtual void Clear(){};
  virtual void WriteOut();

  ClassDef(NewTree_Analyzer,0);
};


#endif
