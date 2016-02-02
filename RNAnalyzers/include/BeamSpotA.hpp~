/***********************************************************
//Class: ReconstructBeamSpotA
//
//Author:Sean Kuvin
//Original Date: 8/28/2014
//
//Simple method A for reconstructing the beamspot using 
//the IonChamber hit position.  The only ansatz to be made
//here is a scale factor to account for dispersion from the
//target position to ionchamber.
//In your script, use (for ex:)
//
//> beamspotA::ReconstructBeamSpotA beamanalyzerA;
//> beamanalyzerA.SetDispersionScaleFactor(0.3);
//> gAnalyzer_stack.Add(&beamanalyzerA)
//make sure your config file has the appropriate detector distances
//and calibrations (silicon and IC)
*******************************************************************/


#ifndef _RECOBEAMSPOTA_H
#define _RECOBEAMSPOTA_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


namespace beamspotA{


class ReconstructBeamSpotA:public RN_Analyzer{
private:
  TVector3 fBeamSpot;
  TVector3 fSiAVector;
  TVector3 fSiBVector;
  Double32_t fSiAAngle;
  Double32_t fSiBAngle;
  Double32_t fErProton;
  Double32_t fDispersion;
  
  
  
  
public:  
  ReconstructBeamSpotA();
  virtual ~ReconstructBeamSpotA(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual void Reset();
  void SetDispersionScaleFactor(double factor);

  ClassDef(ReconstructBeamSpotA,0);
};
  //declare global parameters as extern


  void LoadBeamSpotAGates();
  
  
}
#endif
