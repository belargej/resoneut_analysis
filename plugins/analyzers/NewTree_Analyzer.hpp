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

  
  virtual bool Begin();
  virtual bool Process();
  virtual bool Terminate();
  virtual void Clear(){};
  virtual void ResetGlobals();

  ClassDef(NewTree_Analyzer,0);
};


#endif
