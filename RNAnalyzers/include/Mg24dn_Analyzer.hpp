/***********************************************************
//Class: Mg24dn_Analyzer
//
//Author:Sean Kuvin
//
***********************************************************/

#ifndef _Mg24DNANALYZER_H
#define _Mg24DNAnalyzer_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


namespace _Mg24{


class Mg24dn_Analyzer:public RN_Analyzer{
private:

public:  
  Mg24dn_Analyzer();
  virtual ~Mg24dn_Analyzer(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void Reset();





  ClassDef(Mg24dn_Analyzer,0);
};

void Load_Mg24_dn_Gates();


}
#endif
