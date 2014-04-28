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
  UInt_t Reset();
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




class RN_Parameter_Stack:public RN_BaseClass{
protected:
  TList *fRNParameters;//!  
public: 
 
  RN_Parameter_Stack(const TString& name="");
  ~RN_Parameter_Stack(){
    if(fRNParameters){
      ClearStack();
      delete fRNParameters;
      fRNParameters=NULL;
    }
  }
  void Init();
  UInt_t GetSize()const{return fRNParameters ? fRNParameters->GetSize() : 0;}
  UInt_t GetNum() const{return GetSize();}
  virtual UInt_t AddBranches(TTree* _tree);
  virtual UInt_t SetBranches(TTree* _tree);
  UInt_t AddParameter(RN_Parameter *param);
  void ClearStack(){if(fRNParameters)fRNParameters->Clear();} 
  virtual void Print();
  UInt_t Reset();
  

  ClassDef(RN_Parameter_Stack,1);
};


#endif
