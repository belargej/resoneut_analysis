#ifndef _TRIGANALYZER
#define _TRIGANALYZER
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"
#include "plugins/sak/sak_Histograms.hpp"
#include "plugins/sak/sak_Gates.hpp"


class Trigger_Analyzer:public RN_Analyzer{
private:

public:  
  Trigger_Analyzer();
  virtual ~Trigger_Analyzer(){};

  virtual void Begin();
  virtual void Process();
  virtual void Clear(){};
  virtual void Terminate();


  ClassDef(Trigger_Analyzer,0);
};


#endif
