/***********************************************************/
//Class: Module_Analyzer
//
//Author:Sean Kuvin
//
//Module_Analyzer serves the simple purpose of creating 1D histograms
//for all of the module parameters (just as SpecTcl does)
/***********************************************************/

#ifndef _MODULEANALYZER_H
#define _MODULEANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"




class Module_Analyzer:public RN_Analyzer{
private:

public:  
  Module_Analyzer();
  virtual ~Module_Analyzer(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast(){return 1;};
  virtual void Reset();


  ClassDef(Module_Analyzer,0);
};




#endif
