/************************************************************************
Class: RN_Base Detector
Author: Sean Kuvin                             


 RN_BaseDetector class: Inserting a hit (energy, time, channel) 
 is sorted by energy(default) and mult is counted. 
 Adapted from SpecTcl DetectorParameters.cpp (Diffenderfer, Wiedenhoever)

***********************************************************************/

#ifndef __BaseDetector__h
#define __BaseDetector__h
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <TObject.h>
#include <TMath.h>
#include <TString.h>
#include "RN_VariableMap.hpp"



class RN_BaseDetector:public TObject{
private:
  std::string fName;//!
  int fNumOfCh;//!
  int sorted_by_channel;//!
  double lowlimit;//!
  double highlimit;//!
  std::vector<Double32_t>fCh_cal;//!

public:
  RN_BaseDetector(){
  }
  RN_BaseDetector(std::string name, int num);

  int fMult;
  
  std::vector<Double32_t>fChlist;//[fMult]
  std::vector<Double32_t>fE;//[fMult]
  std::vector<Double32_t>fT;//[fMult]

  
  Double32_t Ch(int i=0)const {return fMult>0 ? fCh_cal[(int)fChlist[i]] : -1 ;}
  void SetELimits(const double&,const double&);
  void Init(const double& num);
  void Reset();
  Int_t NumOfCh()const{return fNumOfCh;}
  std::string Name()const {return fName;} 
  int InsertHit(const double&, const double&, const double&);
  void SetSortByChannel(){sorted_by_channel=1;}
  virtual void SetCalibrations(RN_VariableMap& detvar);

  ClassDef(RN_BaseDetector,1);
};


typedef std::vector<RN_BaseDetector> RN_BaseCollection;
typedef std::vector<RN_BaseDetector>::iterator RN_BaseCollectionRef;




#endif
