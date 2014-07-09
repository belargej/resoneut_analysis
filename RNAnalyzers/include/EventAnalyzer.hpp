/***********************************************************/
//Class: EventAnalyzer
//
//Author:Sean Kuvin
//
//EventAnalyzer class to be read out basic information from the
//event database
/***********************************************************/

#ifndef _EVENTANALYZER_H
#define _EVENTANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"
#include "TTree.h"

namespace eventinfo{

class EventAnalyzer:public RN_Analyzer{
private:
  Int_t fRunMin;
  Int_t fRunMax;
public:
  
  EventAnalyzer();
  virtual ~EventAnalyzer(){};

  
  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual bool Terminate();
  virtual void Clear(){};
  virtual void Reset();
  void SetRunMinMax(int min, int max);

  ClassDef(EventAnalyzer,0);
};

}
#endif
