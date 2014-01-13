/***********************************************************
//Class: S1TestClass
//
//Author:Sean Kuvin
//
***********************************************************/

#ifndef _ANALYZERTEMPLATE_H
#define _ANALYZERTEMPLATE_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "../include/RN_Analyzer.hpp"


//namespace template{


class S1TestClass:public RN_Analyzer{
private:

public:  
  S1TestClass();
  virtual ~S1TestClass(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void ResetGlobals();





  ClassDef(S1TestClass,0);
};
//declare global parameters as extern


void LoadS1TestGates();



#endif
