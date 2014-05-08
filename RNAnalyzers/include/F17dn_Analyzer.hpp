/***********************************************************
//Class: F17dn_Analyzer
//
//Author:Sean Kuvin
//
***********************************************************/

#ifndef _F17DNANALYZER_H
#define _F17DNAnalyzer_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


namespace _F17{


class F17dn_Analyzer:public RN_Analyzer{
private:

public:  
  F17dn_Analyzer();
  virtual ~F17dn_Analyzer(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void ResetGlobals();





  ClassDef(F17dn_Analyzer,0);
};

void Load_F17_dn_Gates();


}
#endif
