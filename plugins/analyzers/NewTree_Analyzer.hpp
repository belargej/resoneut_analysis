#ifndef _NTANALYZER
#define _NTANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


class NewTree_Analyzer:public RN_Analyzer{
private:

public:
  
  NewTree_Analyzer();
  virtual ~NewTree_Analyzer(){};

  void InitTree(std::string filename,std::string treename);
  virtual void Process();
  virtual void Clear(){};
  virtual void Terminate();


  ClassDef(NewTree_Analyzer,0);
};


#endif
