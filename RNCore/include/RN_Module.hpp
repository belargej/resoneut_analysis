//////////////////////////////////////////////////////////////
/// RN_Module for handling the modules needed for the unpacker.
///
///
/// Author: Sean Kuvin
//////////////////////////////////////////////////////////////


#ifndef __RN_MODULE_H_
#define __RN_MODULE_H_
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

#define MODULE_MAX_CHANNELS 32

//use these types for checking how many of a module type are in the stack
//stack.GetNum(kCAENtype) or stack.GetNum(kMESYtype) for 
const static unsigned int kCAENtype(1);
const static unsigned int kMESYtype(2);

//RN_Module is an abstract baseclass, example modules which inherit from RN_Module and override the proper methods are CAEN_ADC and MESY_QDC
class RN_Module:public RN_BaseClass{
protected:

  UShort_t fNumOfCh;//!
  UInt_t fZeroSuppression;//!
  UInt_t fValid;//!
  UInt_t fModuleType;//!
  UShort_t fGeoAddress;//!
  UInt_t fCounter;//!

public:
  TBranch * b_module;
  Float_t fChValue[MODULE_MAX_CHANNELS];

 
  RN_Module(const TString& name = "",const UInt_t& geoaddress = 0);
  ~RN_Module(){};

  Float_t operator () (UInt_t i) const {
    return ( i<fNumOfCh ? fChValue[i] : 0 ) ;  
  }
  
  Float_t operator [] (UInt_t i) const {
    return operator()(i) ;  
  }

  virtual Bool_t Unpack(unsigned short *& gpointer) = 0;
  UShort_t NumOfCh()const {return fNumOfCh;}
  UShort_t GeoAddress() const{return fGeoAddress;}
  UInt_t GetCounter()const {return fCounter;}
  UInt_t ModuleType()const {return fModuleType;}
  UInt_t ZeroSuppression()const {return fZeroSuppression;}
  UInt_t CheckValid()const {return fValid;}
  UInt_t SortChVal(const UInt_t& ch, const UInt_t& val);
  virtual void Reset();
  virtual UInt_t AddBranch(TTree* _tree);
  virtual UInt_t SetBranch(TTree* _tree);
  virtual void Print(); 

  ClassDef(RN_Module,1);
};

class RN_Module_Stack:public RN_BaseClass_Stack{
protected:

public: 
 
  RN_Module_Stack(const TString& name="");
  ~RN_Module_Stack(){}
  
  
  UInt_t GetNum(UInt_t modtype = 0);
  virtual UInt_t AddBranches(TTree* _tree);
  virtual UInt_t SetBranches(TTree* _tree); 
  virtual UInt_t Add(RN_BaseClass * base);
  Bool_t UnpackModules(unsigned short *& pointer, int filepos);
  UInt_t SortGeoChVal(const UShort_t&geoaddress,const UInt_t& ch, const UInt_t& val);
 
  ClassDef(RN_Module_Stack,1);
};

extern RN_Module_Stack gModule_stack;


class CAEN_ADC:public RN_Module{
private:
public:

  CAEN_ADC(){};
  CAEN_ADC(const TString& name,const UInt_t& geoaddress);

  virtual Bool_t Unpack(unsigned short*& gpointer);
  ClassDef(CAEN_ADC,1);
};

class CAEN_TDC:public CAEN_ADC{
private:
public:

  CAEN_TDC(){};
  CAEN_TDC(const TString& name,const UInt_t& geoaddress):CAEN_ADC(name,geoaddress){};

  ClassDef(CAEN_TDC,1);
};

class MESY_QDC:public RN_Module{
private:
public:
  MESY_QDC(){};
  MESY_QDC(const TString& name,const UInt_t& geoaddress);
  virtual Bool_t Unpack(unsigned short*& gpointer);


  ClassDef(MESY_QDC,1);
};


#endif
