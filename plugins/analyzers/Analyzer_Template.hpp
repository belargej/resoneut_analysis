/***********************************************************
//Class: Analyzer_Template
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


class Analyzer_Template:public RN_Analyzer{
private:

public:  
  Analyzer_Template();
  virtual ~Analyzer_Template(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void ResetGlobals();





  ClassDef(Analyzer_Template,0);
};
//declare global parameters as extern


void LoadTemplateGates();



#endif
