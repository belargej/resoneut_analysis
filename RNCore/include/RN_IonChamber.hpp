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
  TVector3 fHitPos;//!
  Double32_t fZPos;//!
  Double32_t fYPos;//!
  Double32_t fXPos;//!
  Double32_t fWireDist;//!

 public:
  RN_IonChamber(std::string name="ic"):RN_BaseClass(name,name),
				       fHitPos(TVector3(0,0,0)),
				       fZPos(250),
				       fYPos(0),
				       fXPos(0),
				       fWireDist(2),//mm
				       esegment(name+".esegment",1),
				       desegment(name+".desegment",1),
				       xgrid(name+".xgrid",32),
				       ygrid(name+".ygrid",32)
  {
  }

  RN_BaseDetector esegment;
  RN_BaseDetector desegment;
  RN_BaseDetector xgrid;
  RN_BaseDetector ygrid;

  
  std::string Name()const{return GetName();}//!
  void Reset();
  void SetCalibrations(RN_VariableMap& detvar);
  Double32_t SumE_X() const;//sum all wires xgrid section
  Double32_t SumE_Y() const;//sum all wires ygrid section
  Double32_t Pos_X();
  Double32_t Pos_Y();
  void ReconstructHitPos();
  TVector3 GetHitPos() const {return fHitPos;}
  Double32_t Theta() const {return fHitPos.Theta() * 180 / TMath::Pi();}
  Double32_t Phi() const {return fHitPos.Phi() * 180 / TMath::Pi() ;}

  Double32_t E() const ;
  Double32_t DE() const ;
  Double32_t ERaw() const ;
  Double32_t DERaw() const ;
  Double32_t TotalE() const; 
  Double32_t SumE_Pos()const;
  Double32_t T() const ;
  Double32_t TRaw() const ;
 
  ClassDef(RN_IonChamber,1);
  
};
inline Double32_t RN_IonChamber::ERaw()const {return esegment.ERaw();}
inline Double32_t RN_IonChamber::DERaw()const {return desegment.ERaw();}
inline Double32_t RN_IonChamber::E() const {return esegment.E();}
inline Double32_t RN_IonChamber::DE() const {return desegment.E();}
inline Double32_t RN_IonChamber::T() const {return esegment.T();}
inline Double32_t RN_IonChamber::TRaw() const {return esegment.TRaw();}
inline Double32_t RN_IonChamber::TotalE() const{return E()+DE();}//sum-all sections
inline Double32_t RN_IonChamber::SumE_Pos()const {return SumE_X()+SumE_Y();}//sum-both pos grid sections

#endif
