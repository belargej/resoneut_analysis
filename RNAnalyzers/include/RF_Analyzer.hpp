/***********************************************************
//Class: Analyzer_Template
//
//Author:Sean Kuvin
//
***********************************************************/

#ifndef _RFANALYZER_H
#define _RFANALYZER_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


namespace RF_analyzer{


  class RF_Analyzer:public RN_Analyzer{
  private:

  public:  
    RF_Analyzer();
    virtual ~RF_Analyzer(){};

    virtual bool Begin();
    virtual bool Process();
    virtual bool ProcessFill();
    virtual void Clear();
    virtual bool Terminate();
    virtual bool TerminateIfLast();
    virtual void Reset();





    ClassDef(RF_Analyzer,0);
  };

  extern TCutG* RF_Cut1;
  extern TCutG* RF_Cut2;
  extern TCutG* RF_Cut3;

  extern int rf_check[3];
  void RequireRF1();
  void RequireRF2();
  void RequireRF3();
  
  void LoadGates(const std::string &input);
  void ClearGates();  
}

#endif
