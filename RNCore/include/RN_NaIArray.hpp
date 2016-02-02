/***********************************************************************
Class: RN_NaIDetector, RN_NaIArray

RN_NaIDetector stores the raw data for the NaI detectors.  
The data indices 0 and 1 refer to the upstream and downstream 
sides of the detectors, respectively.  Therefore fE[0] stores the upstream
Energy and elin[0] and eshift[0] give the Energy E1().

RN_NaICollection is an std::vector of NaIDetectors.  This collection is passed to the RN_NaIArray for the ReconstructHits method.

fE and fT are public so they are set directly:
RN_NaICollection nai_;
RN_NaIArray nai_array;
nai_.push_back(RN_NaIDetector("nai_left1");
nai_[0].fE[0]=100;
nai_[0].fE[1]=200;
nai_array.ReconstructHits(nai_);


Author: Sean A Kuvin -2013
************************************************************************/

#ifndef __RNNAI__H
#define __RNNAI__H
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


static const Double_t light_atten = 0.047; //cm^-1

class RN_NaIDetector:public RN_BaseClass{
private:  
  Double32_t elin[2];//!  //up-0 down-1
  Double32_t eshift[2];//! 
  Double32_t tlin[2];//!
  Double32_t tshift[2];//!
  Double32_t tzero[2];//! 


public:

  Double32_t fE[2]; // 0 -upstream energy, 1 = downstream energy
  Double32_t fT[2];

  RN_NaIDetector(){}
  RN_NaIDetector(std::string name):RN_BaseClass(name,name)
  {
    for(int i=0;i<2;i++){
      tzero[i]=0;
      tlin[i]=1;
      tshift[i]=0;
      elin[i]=1;
      eshift[i]=0;
    }
    Reset();
  }

  inline Double32_t TZero(int id) const{return tzero[id];}
  inline Double32_t T1()   const{ return fT[0]>0 ? (fT[0] * tlin[0] + tshift[0]):0; }
  inline Double32_t T2()   const{ return fT[1]>0 ? (fT[1] * tlin[1] + tshift[1]):0; }

  //T(int i) is deprecated.  still included so that some analyzers won't crash
  inline Double32_t T(int i=0)   const
  {
    /* if (T1())
      return(T1());
    else if (T2())
      return(T2());
    else return -5000;
    */
    if ((T1()>=T2() || !T1()) && T2())
      return T2();
    else if ((T1()<T2() || !T2()) && T1())
      return T1();
    else 
      return -5000;
    
  }
  inline Double32_t E1()   const{ return fE[0]>0 ? (fE[0] * elin[0] + eshift[0]):0; }
  inline Double32_t E2()   const{ return fE[1]>0 ? (fE[1] * elin[1] + eshift[1]):0;}
  inline Double32_t SumE() const{return E1()+E2();};
  inline Double32_t E_Gamma() const{return TMath::Sqrt(E1()*E2());}
  Double32_t Position() const;

  void Reset();
  void SetCalibrations(RN_VariableMap& detvar);

  
  ClassDef(RN_NaIDetector,0);

};

typedef std::vector<RN_NaIDetector> RN_NaICollection;
typedef std::vector<RN_NaIDetector>::iterator RN_NaICollectionRef;
typedef std::vector<RN_NaIDetector>::const_iterator RN_NaICollectionCRef;


class RN_NaIArray:public RN_BaseDetector{
 private:
  Double32_t elin;//!
  Double32_t eshift;//!
  Double32_t tlin;//!
  Double32_t tshift;//!

  Double32_t tfirst;
  Int_t t_mult;
  Int_t detfirst; 
  Double32_t efirst;
  //Double32_t ESum;
  
 public:
  
  
  //Double32_t fESum;
  std::vector<Double32_t> fPosition;
  //Double32_t fDetNumber;

  RN_NaIArray(){}
  RN_NaIArray(std::string name, int num = 20):  RN_BaseDetector(name,num),
						elin(1),
						eshift(0),
						tlin(1),
						tshift(0),	
						tfirst(4096),
						t_mult(0),
						efirst(-1),
						fPosition(20,(double)0.)//,
						//fDetNumber(-1)
  {
  } 
  void Reset();
  void SetCalibrations(Double32_t, Double32_t, Double32_t, Double32_t);
  void SetCalibrations(RN_VariableMap& detvar);
  int FindTFirst(const double& t);
  int FindTFirst(const double& t,const int& det);
  int FindTFirst(const double& t,const int& det, const double& e);
  inline Double32_t TFirst() const{return tfirst;};
  inline Double32_t EFirst() const{return efirst;};
  inline Int_t DetFirst() const{return detfirst;};
  inline Double32_t TMult() const{return t_mult;};
  void ReconstructHits(const RN_NaICollection&in);
  int InsertHit(const Double32_t&,const Double32_t&,const Double32_t&,const Int_t&);

  //inline Double32_t GetESum() const{ return ESum;};

  //Double32_t NaIDetNumber(unsigned int i=0)

  inline Double32_t Phi(int i=0)const{return 0;} ;




  ClassDef(RN_NaIArray,1);
  
};



#endif
