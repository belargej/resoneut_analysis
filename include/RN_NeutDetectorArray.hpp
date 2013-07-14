#ifndef __NEUTARRAY__
#define __NEUTARRAY__
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




class RN_NeutDetector:public RN_BaseDetector{
 private:
  double elin;//!
  double eshift;//!
  double tlin; //!
  double tshift;//!
  double zero_off;//!
  int apos;
  TVector3 pos_vect;//!
  TVector3 fPos;
 
 public:
  RN_NeutDetector(){}
  RN_NeutDetector(std::string name,int num,int ap):RN_BaseDetector(name,num),
						   apos(ap)
  {
    DeterminePosition(apos);
  } 

  ~RN_NeutDetector(){}

  //NeutDetector Specific Parameterss 
  Double_t fQ_long;
  Double_t fQ_short;
  Double_t fPSD;
  Double_t fTrel;

  void InsertHit(const double&,const double&);
  Double_t PSD() const ;
  Double_t Q() const ;
  bool DeterminePosition(int apos); 
  void Reset();
  void SetCalibrations(double elin, 
		       double eshift,
		       double tlin,
		       double tshift,
		       double zero_off);
  void SetCalibrations(RNVariableMap& detvar);
  void ApplyCalibrations();
  double CalculateTRel(const std::vector<RN_NeutDetector>&ndet,double &tfirst);

  
  ClassDef(RN_NeutDetector,1);
};



#endif
