/***********************************************************
//Class: QValCut
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
#include "RN_Analyzer.hpp"


//namespace template{


class QValCut:public RN_Analyzer{
private:

public:  
  QValCut();
  virtual ~QValCut(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void Reset()()();





  ClassDef(QValCut,0);
};
//declare global parameters as extern


void LoadTemplateGates();



#endif
