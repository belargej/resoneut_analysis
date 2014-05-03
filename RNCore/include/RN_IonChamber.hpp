//////////////////////////////////////////////////////////////
///This class to be modified using LSU methods for the ion chamber
////////////////////////////////////////////////////////////////////

#ifndef __RNIONC__
#define __RNIONC__
//C and C++ libraries.
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>

//ROOT libraties
#include <TString.h>
#include <TFile.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TChain.h>
#include <TObject.h>
#include <TLine.h>
#include <TTree.h>
#include <TBranch.h>
#include <TRandom3.h>
#include <TPolyLine3D.h>
#include <TVector3.h>

#include "RN_VariableMap.hpp"
#include "RN_BaseDetector.hpp"



class RN_IonChamber:public RN_BaseClass{
 private:
  float elin;//!
  float eshift;//!
  float tlin;//!
  float tshift;//!
  TVector3 fHitPos;//!
  Double32_t _zpos;//!
  Double32_t _ypos;//!
  Double32_t _xpos;//!
  Double32_t wire_dist;//!
 public:
  RN_IonChamber(){}
  RN_IonChamber(std::string name):RN_BaseClass(name,name),
				  elin(1),
				  eshift(0),
				  tlin(1),
				  tshift(0),
				  fHitPos(TVector3(0,0,0)),
				  _zpos(250),
				  _ypos(0),
				  _xpos(0),
				  wire_dist(5),//mm
				  fE(0),
				  fdE(0),
				  xgrid(name+".xgrid",32),
				  ygrid(name+".ygrid",32)
					 
  {
  }
  Double32_t fE;
  Double32_t fdE;
  Double32_t fT;
  RN_BaseDetector xgrid;
  RN_BaseDetector ygrid;

  
  std::string Name()const{return GetName();}//!
  void Reset();
  void SetCalibrations(float, float, float, float);
  void SetCalibrations(RN_VariableMap& detvar);
  Double32_t E() const {return fE>0 ? (fE * elin + eshift) : 0;}
  Double32_t DE() const {return fdE>0 ? (fdE * elin + eshift) : 0;}
  Double32_t TotalE() const{return E()+DE();}//sum-all sections
  Double32_t SumE_Pos()const {return SumE_X()+SumE_Y();}//sum-both pos grid sections
  Double32_t SumE_X() const;//sum all wires xgrid section
  Double32_t SumE_Y() const;//sum all wires ygrid section
  Double32_t Pos_X();
  Double32_t Pos_Y();
  void ReconstructHitPos();
  TVector3 GetHitPos() const {return fHitPos;}

  inline Double32_t T() const{return fT>0 ? ((fT * tlin) + tshift): 0;}
  Double32_t Theta() const {return fHitPos.Theta() * 180 / TMath::Pi();}
  Double32_t Phi() const {return fHitPos.Phi() * 180 / TMath::Pi() ;}

  ClassDef(RN_IonChamber,1);
  
};




#endif
