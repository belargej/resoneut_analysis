#ifndef _MODULEANALYZER_H
#define _MODULEANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"



class Module_Analyzer:public RN_Analyzer{
private:

public:  
  Module_Analyzer();
  virtual ~Module_Analyzer(){};

  virtual void Begin();
  virtual void Process();
  virtual void Clear(){};
  virtual void Terminate();
  virtual void ResetGlobals();


  ClassDef(Module_Analyzer,0);
};




#endif
