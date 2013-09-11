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

class RN_NaIDetector:public TObject{
private:  
  std::string fName;
  float elin;//!
  float eshift;//!
  float tlin;//!
  float tshift;//!
public:

  Double32_t fE[2];
  Double32_t fT[2];

  RN_NaIDetector(){}
  RN_NaIDetector(std::string name):fName(name),
				   elin(1),
				   eshift(0),
				   tlin(1),
				   tshift(0)
  {
    Reset();
  }


  std::string Name(){return fName;}
  void Reset();
  void ApplyCalibrations(){}
  void SetCalibrations(float, float, float, float);
  void SetCalibrations(RN_VariableMap& detvar);
  double SumE() const{return fE[0]+fE[1];};

  ClassDef(RN_NaIDetector,0);

};

typedef std::vector<RN_NaIDetector> RN_NaICollection;
typedef std::vector<RN_NaIDetector>::iterator RN_NaICollectionRef;

class RN_NaIArray:public RN_BaseDetector{
 private:
  float elin;//!
  float eshift;//!
  float tlin;//!
  float tshift;//!
 public:
  RN_NaIArray(){}
  RN_NaIArray(std::string name):  elin(1),
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
