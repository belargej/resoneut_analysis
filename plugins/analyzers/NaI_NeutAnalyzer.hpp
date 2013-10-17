#ifndef _NAI_NEUTANALYZER_H
#define _NAI_NEUTANALYZER_H
#include <TH2.h>
#include <TFile.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"
#include "../../include/RN_Root.hpp"


class NaI_NeutAnalyzer:public RN_Analyzer{
private:
  
public:
  
  NaI_NeutAnalyzer();
  virtual ~NaI_NeutAnalyzer(){};
  
  virtual bool Begin();
  
  virtual bool Process();
  virtual bool Terminate();
  
  virtual void Clear(){};
  virtual void ResetGlobals();

  
  ClassDef(NaI_NeutAnalyzer,0);
  
};






#endif
