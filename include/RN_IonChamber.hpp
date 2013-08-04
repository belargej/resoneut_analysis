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

#define I_CN 5 //ion chan num



class RN_IonChamber:public TObject{
 private:
  std::string fName;//!
  float elin;//!
  float eshift;//!
  float tlin;//!
  float tshift;//!
 public:
  RN_IonChamber(){}
  RN_IonChamber(std::string name,int num = 2):fName(name),
					 elin(1),
					 eshift(0),
					 tlin(1),
					 tshift(0),
					 ede("ede",num),
					 xgrid("xgrid",32),
					 ygrid("ygrid",32)
					 
  {
  }
  RN_BaseDetector ede;
  RN_BaseDetector xgrid;
  RN_BaseDetector ygrid;

  
  std::string Name()const{return fName;}
  void Reset();
  void ApplyCalibrations(){}
  void SetCalibrations(float, float, float, float);
  void SetCalibrations(RN_VariableMap& detvar);
 
  ClassDef(RN_IonChamber,1);
  
};


#endif