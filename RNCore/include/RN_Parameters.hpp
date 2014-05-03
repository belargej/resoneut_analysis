//////////////////////////////////////////////////////////////
/// RN_Parameters for handling the experiment parameters.
///
///
/// Author: Sean Kuvin
//////////////////////////////////////////////////////////////


#ifndef __RN_PARAMETERS_H_
#define __RN_PARAMETERS_H_
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
#include <TH2.h>
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
#include <TLorentzVector.h>
#include <TCutG.h>

#include "RN_BaseClass.hpp"

class RN_Parameter:public RN_BaseClass{
protected:

  UInt_t fBins;//!
  UInt_t fMin;//!
  UInt_t fMax;//!
  Bool_t fValid;//!

public:
  TBranch * b_param;
  Double32_t fValue;

 
  RN_Parameter(const TString& name,const int &bins = 4096,const Double32_t& min = 0, const Double32_t& max = 4095);
  ~RN_Parameter(){};

  Bool_t operator == (const Double32_t& val) const ;
  Bool_t operator == (const RN_Parameter& param) const ;
  Bool_t operator != (const RN_Parameter& param) const ;
  Bool_t operator != (const Double32_t& val) const ;
  Double32_t operator = (const Double32_t& val);
  Double32_t operator = (const RN_Parameter& param);
  Double32_t operator  += (const Double32_t &val);
  Double32_t operator  -= (const Double32_t &val);
  Double32_t operator  /= (const Double32_t &val);
  Double32_t operator  *= (const Double32_t &val);
  Double32_t operator  += (const RN_Parameter &param);
  Double32_t operator  -= (const RN_Parameter &param);
  Double32_t operator  /= (const RN_Parameter &param);
  Double32_t operator  *= (const RN_Parameter &param);


  Bool_t CheckValid()const {return fValid;}  
  virtual void Reset();
  virtual UInt_t AddBranch(TTree* _tree);
  virtual UInt_t SetBranch(TTree* _tree);
  virtual void Print(); 

  ClassDef(RN_Parameter,1);
};

  Double32_t operator * (const RN_Parameter& a, const RN_Parameter& b);
  Double32_t operator * (const Double32_t& a, const RN_Parameter& b);
  Double32_t operator * (const RN_Parameter& a, const Double32_t& b);
  Double32_t operator / (const RN_Parameter& a, const RN_Parameter& b);
  Double32_t operator / (const Double32_t& a, const RN_Parameter& b);
  Double32_t operator / (const RN_Parameter& a, const Double32_t& b);
  Double32_t operator + (const RN_Parameter& a, const RN_Parameter& b);
  Double32_t operator + (const Double32_t& a, const RN_Parameter& b);
  Double32_t operator + (const RN_Parameter& a, const Double32_t& b);
  Double32_t operator - (const RN_Parameter& a, const RN_Parameter& b);
  Double32_t operator - (const Double32_t& a, const RN_Parameter& b);
  Double32_t operator - (const RN_Parameter& a, const Double32_t& b);




class RN_Parameter_Stack:public RN_BaseClass_Stack{
protected:

public: 
 
  RN_Parameter_Stack(const TString& name="");
  ~RN_Parameter_Stack(){}

  virtual UInt_t AddBranches(TTree* _tree);
  virtual UInt_t SetBranches(TTree* _tree);
  virtual UInt_t Add(RN_BaseClass * base);

  ClassDef(RN_Parameter_Stack,1);
};


extern RN_Parameter_Stack gParameter_stack;

#endif
