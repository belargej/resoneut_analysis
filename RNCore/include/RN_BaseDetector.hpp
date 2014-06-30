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
protected:
  int fNumOfCh;//!
  int fSortedByChannel;//!
  double fLowLimit;//!
  double fHighLimit;//!
  std::vector<Double32_t>fChCal;//!
  std::vector<Double32_t>fA0;//!
  std::vector<Double32_t>fA1;//!
  std::vector<Double32_t>fT0;//!
  std::vector<Double32_t>fT1;//!
  std::vector<Double32_t>fQOffset;//!
  std::vector<Double32_t>fTOffset;//!
  Double_t fELin;
  Double_t fEShift;
  Double_t fTLin;
  Double_t fTShift;

  //parameters
  UInt_t fMult;
  std::vector<Double32_t>fChlist;//[fMult]
  std::vector<Double32_t>fE;//[fMult]
  std::vector<Double32_t>fT;//[fMult]
  
public:
  RN_BaseDetector(){
  }
  RN_BaseDetector(std::string name, int num);

  UInt_t Mult() const;
  Double32_t Ch(unsigned int i=0) const ;  
  Double32_t ChRaw(unsigned int i=0) const ;  
  std::vector<Double32_t> ChList() const ;  
  Double32_t ERaw(unsigned int i=0) const;
  Double32_t EOffset(unsigned int i=0)const ; //energy with offset
  Double32_t ELocal(unsigned int i=0)const ; //local gain matching calibrations with offset
  Double32_t E(unsigned int i=0)const ; //local and global with offset
  Double32_t TRaw(unsigned int i=0) const;
  Double32_t TOffset(unsigned int i=0) const ;// time with offset
  Double32_t TLocal(unsigned int i=0) const ;// local time calibrations
  Double32_t T(unsigned int i=0) const ;// global time calibrations
  

  void SetELimits(const double&,const double&);
  void Init(const double& num);
  void Reset();
  Int_t NumOfCh()const{return fNumOfCh;}
  std::string Name()const {return GetName();} 
  int InsertHit(const double&, const double&, const double&);
  void SetSortByChannel(bool val = true);
  virtual void SetCalibrations(RN_VariableMap& detvar);
  virtual void Print();

  ClassDef(RN_BaseDetector,1);
};

inline UInt_t RN_BaseDetector::Mult() const { return fMult;}

inline Double32_t RN_BaseDetector::ERaw(unsigned int i) const{
  return ((i<fMult) ? fE[i] : 0);
}
inline Double32_t RN_BaseDetector::TRaw(unsigned int i) const{
  return ((i<fMult) ? fT[i] : 0);
}
inline Double32_t RN_BaseDetector::ChRaw(unsigned int i) const{
  return ((i<fMult) ? fChlist[i] : -1);
}

inline std::vector<Double32_t> RN_BaseDetector::ChList() const{
  return fChlist;
}

/********************************************************************/
#ifdef __BaseDetector__CXX  

 //calibration functions
Double32_t RN_BaseDetector::Ch(unsigned int i)const 
{
  return ((i<fMult) ? fChCal[(int)fChlist[i]] : -1 );
}
Double32_t RN_BaseDetector::EOffset(unsigned int i)const 
{
  return ((i<fMult) ? (fE[i] + fQOffset[(int)fChlist[i]]) : 0);
}

Double32_t RN_BaseDetector::ELocal(unsigned int i)const 
{
  return ((i<fMult) ? ((EOffset(i)* fA1[(int)fChlist[i]]) + fA0[(int)fChlist[i]]) : 0);
}

Double32_t RN_BaseDetector::E(unsigned int i)const 
{
  return ((i<fMult) ? (ELocal(i)*fELin + fEShift) : 0);
}

Double32_t RN_BaseDetector::TOffset(unsigned int i)const 
{
  return ((i<fMult) ? (fT[i] + fTOffset[(int)fChlist[i]]) : 0);
}

Double32_t RN_BaseDetector::TLocal(unsigned int i)const 
{
  return ((i<fMult) ? ((TOffset(i)* fT1[(int)fChlist[i]]) + fT0[(int)fChlist[i]]) : 0);
}

Double32_t RN_BaseDetector::T(unsigned int i)const 
{
  return ((i<fMult) ? (TLocal(i)*fTLin + fTShift) : 0);
}

void RN_BaseDetector::SetSortByChannel(bool val)
{
  fSortedByChannel = val;
}

#endif


typedef std::vector<RN_BaseDetector> RN_BaseCollection;
typedef std::vector<RN_BaseDetector>::iterator RN_BaseCollectionRef;




#endif
