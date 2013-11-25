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


static const Double_t light_atten = 0.047; //cm^-1

class RN_NaIDetector:public TObject{
private:  
  std::string fName;
  Double32_t elin[2];//!  //up-0 down-1
  Double32_t eshift[2];//!
  Double32_t tlin[2];//!
  Double32_t tshift[2];//!
  Double32_t tzero[2];//! 


public:

  Double32_t fE[2];
  Double32_t fT[2];

  RN_NaIDetector(){}
  RN_NaIDetector(std::string name):fName(name)
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

  inline Double32_t T1()   const{
    if(!fT[0]>0)
      return 0;
    else
      return (fT[0] * tlin[0] + tshift[0]);
  }

  inline Double32_t T2()   const{
    if(!fT[1]>0)
      return 0;
    else
      return (fT[1] * tlin[1] + tshift[1]);
    
  }

  //T(int i) is deprecated.  still included so that some analyzers won't crash
  inline Double32_t T(int i=0)   const{
    if (T1()>=T2() || !T1())
      return T2();
    else if (T1()<T2() || !T2())
      return T1();
    else 
      return 0;
    
  }
  
  inline Double32_t E1()   const{
    if(!fE[0]>0)
    return 0;
    else 
      return (fE[0] * elin[0] +eshift[0]);
    
  }

  inline Double32_t E2()   const{
    if(!fE[1]>0)
      return 0;
    else 
      return (fE[1] * elin[1] +eshift[1]);
  }

  inline Double32_t SumE() const{return E1()+E2();};

  inline Double32_t E_Gamma() const{return TMath::Sqrt(E1()*E2());}
  Double32_t Position() const;

  std::string Name(){return fName;}
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
 public:
 
  std::vector<Double32_t> fPosition;

  RN_NaIArray(){}
  RN_NaIArray(std::string name, int num = 20):  RN_BaseDetector(name,num),
						elin(1),
						eshift(0),
						tlin(1),
						tshift(0),	
						fPosition(20,(double)0.)
  {
  } 
  void Reset();
  void SetCalibrations(Double32_t, Double32_t, Double32_t, Double32_t);
  void SetCalibrations(RN_VariableMap& detvar);

  void ReconstructHits(const RN_NaICollection&in);
  int InsertHit(const Double32_t&,const Double32_t&,const Double32_t&,const Int_t&);

  inline Double32_t Phi(int i=0)const{return 0;} ;




  ClassDef(RN_NaIArray,1);
  
};



#endif
