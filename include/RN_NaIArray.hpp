#ifndef __RNNAI__
#define __RNNAI__
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



class RN_NaIArray:public RN_BaseDetector{
 private:
  float elin;//!
  float eshift;//!
  float tlin;//!
  float tshift;//!
 public:
  RN_NaIArray(){}
  RN_NaIArray(std::string name):RN_BaseDetector(name,40),
				  elin(1),
				  eshift(0),
				  tlin(1),
				  tshift(0)
  {
  } 
  void Reset();
  void ApplyCalibrations(){}
  void SetCalibrations(float, float, float, float);
  void SetCalibrations(RN_VariableMap& detvar);
 
  ClassDef(RN_NaIArray,1);
  
};




#endif
