/***********************************************************
//Class: BeamPathLocator
//
//Author: Joe Belarge
//
***********************************************************/

#ifndef _BEAMPATHLOCATOR_H
#define _BEAMPATHLOCATOR_H
#include <TH2.h>
#include <TLine.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


//namespace template{


class BeamPathLocator:public RN_Analyzer{
private:
  int NumberPrefix;
public:  
  BeamPathLocator(int number);
  virtual ~BeamPathLocator(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void Reset()()();

  void SetSiA_XY_Pos(Double32_t xpos, Double32_t ypos);
  void SetSiB_XY_Pos(Double32_t xpos, Double32_t ypos);




  ClassDef(BeamPathLocator,0);
};
//declare global parameters as extern


//void LoadTemplateGates();



#endif
