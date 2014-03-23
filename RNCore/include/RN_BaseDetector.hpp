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



class RN_BaseDetector:public RN_BaseClass{
private:
  int fNumOfCh;//!
  int sorted_by_channel;//!
  double lowlimit;//!
  double highlimit;//!
  std::vector<Double32_t>fCh_cal;//!
  std::vector<Double32_t>a0;//!
  std::vector<Double32_t>a1;//!
  std::vector<Double32_t>t0;//!
  std::vector<Double32_t>t1;//!
  std::vector<Double32_t>q_offset;//!
  std::vector<Double32_t>t_offset;//!

public:
  RN_BaseDetector(){
  }
  RN_BaseDetector(std::string name, int num);

  unsigned int fMult;
  
  std::vector<Double32_t>fChlist;//[fMult]
  std::vector<Double32_t>fE;//[fMult]
  std::vector<Double32_t>fT;//[fMult]

  
  //calibration functions
  inline Double32_t Ch(unsigned int i=0)const {return (i<fMult) ? fCh_cal[(int)fChlist[i]] : -1 ;}
  inline Double32_t E(unsigned int i=0)const {return (i<fMult) ? (((fE[i] + q_offset[(int)fChlist[i]] ) * a1[(int)fChlist[i]]) + a0[(int)fChlist[i]]) : 0;}
  inline Double32_t T(unsigned int i=0)const {return (i<fMult) ? (((fT[i] + t_offset[(int)fChlist[i]] ) * t1[(int)fChlist[i]]) + t0[(int)fChlist[i]]) : 0;}
  
  void SetELimits(const double&,const double&);
  void Init(const double& num);
  void Reset();
  Int_t NumOfCh()const{return fNumOfCh;}
  std::string Name()const {return GetName();} 
  int InsertHit(const double&, const double&, const double&);
  void SetSortByChannel(){sorted_by_channel=1;}
  virtual void SetCalibrations(RN_VariableMap& detvar);

  ClassDef(RN_BaseDetector,1);
};


typedef std::vector<RN_BaseDetector> RN_BaseCollection;
typedef std::vector<RN_BaseDetector>::iterator RN_BaseCollectionRef;




#endif
