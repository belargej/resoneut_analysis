/***********************************************************/
//Class: NewTree_Analyzer
//
//Author:Sean Kuvin
//
//NewTree_Analyzer class to be inserted in the analyzer list for 
//producing a new tree.  Branches are created for the original 
//DataTree format (to allow for immediate reprocessing of the
//new trees with the same analyzers) as some sorted parameters 
//the new trees are filtered down by imposing requirements on the
//analyzers which come before it in the analyzer list.
//for instance, if trigger::Analyzer is first and a RequireS1()
//is set, then NewTree_Analyzer will be filtered to require events
//that have a valid S1 time.
/***********************************************************/

#ifndef _NTANALYZER_H
#define _NTANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"




class NewTree_Analyzer:public RN_Analyzer{
private:

public:
  
  NewTree_Analyzer();
  virtual ~NewTree_Analyzer(){};

  
  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void Clear(){};
  virtual void Reset();

  ClassDef(NewTree_Analyzer,0);
};


#endif
