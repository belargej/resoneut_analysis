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



class RN_IonChamber:public TObject{
 private:
  std::string fName;//!
  float elin;//!
  float eshift;//!
  float tlin;//!
  float tshift;//!
 public:
  RN_IonChamber(){}
  RN_IonChamber(std::string name):fName(name),
				  elin(1),
				  eshift(0),
				  tlin(1),
				  tshift(0),
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

  
  std::string Name()const{return fName;}//!
  void Reset();
  void SetCalibrations(float, float, float, float);
  void SetCalibrations(RN_VariableMap& detvar);
  Double32_t SumE_X();
  Double32_t SumE_Y();
   
  inline Double32_t T() const{return fT>0 ? ((fT * tlin) + tshift): 0;}

  ClassDef(RN_IonChamber,1);
  
};




#endif
