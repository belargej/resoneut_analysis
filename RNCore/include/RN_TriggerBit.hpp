#ifndef __RNTRIGGERBIT__
#define __RNTRIGGERBIT__
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

class RN_TriggerBit:public RN_BaseClass{
private:  
  Double32_t bitmin;
  Double32_t bitmax;
  Double32_t fBit;

public:
  RN_TriggerBit(){}
  RN_TriggerBit(std::string name):RN_BaseClass(name,name),
				  bitmin(0),
				  bitmax(4095),
				  fBit(-1)
    
    
  {
  }

  void SetBit(Double32_t val);
  Double32_t Bit() const;
  std::string Name() const{return GetName();}
  void Reset();
  int Check();
  void SetCalibrations(RN_VariableMap& detvar);
  

  ClassDef(RN_TriggerBit,1);

};

inline Double32_t RN_TriggerBit::Bit() const { return fBit;}


typedef std::vector<RN_TriggerBit> RN_TriggerBitCollection;
typedef std::vector<RN_TriggerBit>::iterator RN_TriggerBitCollectionRef;


#ifdef __RNTRIGGERBIT__CXX

void RN_TriggerBit::SetBit(Double32_t val)
{
  fBit = val;
}

#endif

#endif
