/***********************************************************
//Class: ReconstructBeamSpotB
//
//Author:Sean Kuvin
//Original Date: 8/28/2014
//
//Intermediate method B for reconstructing the beamspot using 
//the IonChamber hit position.  This method follows the same 
//dispersion scale factor but also attempts to use kinematics
//reconstruction(determining expected heavy ion recoil position
//from light particle kinematics in the silicon)
// for further information on beamspot.
***********************************************************/

#ifndef _RECOBEAMSPOTB_H
#define _RECOBEAMSPOTB_H
#include <TH2.h>
#include <cstdlib>
#include <string>
#include <new>
#include "RN_Analyzer.hpp"


namespace beamspotB{


class ReconstructBeamSpotB:public RN_Analyzer{
private:
  TVector3 fRelVector;
  TVector3 fBeamSpot;
  TVector3 fSiAVector;
  TVector3 fSiBVector;
  Double32_t fSiAAngle;
  Double32_t fSiBAngle;
  Double32_t fErProton;
  Double32_t fHDPhi;
  Double32_t fDispersion;
  TLorentzVector fDecayLV;
  TLorentzVector fFragLV;
  TLorentzVector fHeavyDecayLV;


public:  
  ReconstructBeamSpotB();
  virtual ~ReconstructBeamSpotB(){};

  virtual bool Begin();
  virtual bool Process();
  virtual bool ProcessFill();
  virtual void Clear(){};
  virtual bool Terminate();
  virtual void Reset();
  void SetDispersionScaleFactor(double factor);

  ClassDef(ReconstructBeamSpotB,0);
};



void LoadBeamSpotBGates();


}
#endif
