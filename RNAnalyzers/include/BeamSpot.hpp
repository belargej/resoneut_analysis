/***********************************************************
//Class: ReconstructBeamSpot
//
//Author:Sean Kuvin
//
***********************************************************/

#ifndef _RECOBEAMSPOT_H
#define _RECOBEAMSPOT_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


namespace beamspot{


class ReconstructBeamSpot:public RN_Analyzer{
private:

public:  
  ReconstructBeamSpot();
  virtual ~ReconstructBeamSpot(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual bool TerminateIfLast();
  virtual void Reset();

  ClassDef(ReconstructBeamSpot,0);
};
  //declare global parameters as extern


void LoadBeamSpotGates();


}
#endif
