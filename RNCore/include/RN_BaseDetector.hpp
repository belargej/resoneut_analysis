#ifndef __BaseDetector__h
#define __BaseDetector__h

/************************************************************************
Class: RN_BaseDetector
Author: Sean A. Kuvin      

RN_BaseDetector stores the raw data, calibrations, and the methods to apply those calibrations to the raw data.

The data is sorted into the BaseDetector using InsertHit(energy, time, channel) 
which then sorts by energy(default) or by channels and the value fMult is incremented. 
 
Adapted from SpecTcl DetectorParameters.cpp (Diffenderfer, Wiedenhoever).

RN_BaseCollection is a std::vector of BaseDetectors.
Most Detectors inherit directly from RN_BaseDetector but some such as 
RN_S2Detector have 2 RN_BaseDetector members to seperate the theta channels
and phi channels.

***************************************************************************/

#include <iostream>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
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
  inline Double32_t E(unsigned int i=0)const {return (i<fMult) ? ((fabs(fE[i] + q_offset[(int)fChlist[i]] ) * a1[(int)fChlist[i]]) + a0[(int)fChlist[i]]) : 0;}
  inline Double32_t T(unsigned int i=0)const {return (i<fMult) ? ((fabs(fT[i] + t_offset[(int)fChlist[i]] ) * t1[(int)fChlist[i]]) + t0[(int)fChlist[i]]) : 0;}
  
  void SetELimits(const double&,const double&);
  void Init(const double& num);
  void Reset();
  Int_t NumOfCh()const{return fNumOfCh;}
  std::string Name()const {return GetName();} 
  int InsertHit(const double&, const double&, const double&);
  void SetSortByChannel(){sorted_by_channel=1;}
  virtual void SetCalibrations(RN_VariableMap& detvar);
  virtual void Print();

  ClassDef(RN_BaseDetector,1);
};


typedef std::vector<RN_BaseDetector> RN_BaseCollection;
typedef std::vector<RN_BaseDetector>::iterator RN_BaseCollectionRef;




#endif
