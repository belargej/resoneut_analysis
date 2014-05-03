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

#include "RN_VariableMap.hpp"
#include "RN_BaseDetector.hpp"


class RN_NeutDetector:public RN_BaseDetector{
 private:
  double elin;//!
  double eshift;//!
  double tlin; //!
  double tshift;//!
  double zero_off;//!
  int apos;//!
  TVector3 HitPos;//!
  TVector3 fPos;//!
  double phe_lin;//!
  double phe_shift;//!

  //simulation parameters
  double fRadius;//!
  double fThickness;//!
  double fThreshold;//!
  int fCounter;
  int fCounter_carbon;

 public:
  RN_NeutDetector(std::string name="",int num=4,int ap=0);

  ~RN_NeutDetector(){}

  //NeutDetector Specific Parameterss 
  Double32_t fQ_long;
  Double32_t fQ_short;
  Double32_t fPSD;
  Double32_t fT_Q;
  Double32_t fTrel;

  //Simulation Parameters
  Double32_t fDt;
  Double32_t fT_Sim;
  Double32_t fEsum;
  Double32_t fE_lost;
  

  void InsertPSDHit(const double& fql,const double& fqs,const double& t = 0.0);
 
  Double_t PSD() const ;
  Double_t Q_Long() const ;
  Double_t Q_Short_Off() const;
  Double_t E_est() const;
  Double_t T() const;
  Double_t nKE(Double_t tof) const;
  Double_t keVee() const;
  Double_t nKE_R(Double_t tof) const;
  Double_t GetRadius()const {return fRadius;}
  Double_t GetThickness()const {return fThickness;}
  Double_t GetThreshold()const{return fThreshold;}
  TVector3 GetHitPos()const{return HitPos;}
  int HitCounter() const {return fCounter;}
  Int_t GetArrayPos() const{return apos;}
  Double_t Q_value_est(double tof,
		       double m1,
		       double m2,
		       double beam_e,
		       double& hi_KE,
		       double& Q_val);

  TVector3 GetPosVect() const{return fPos;}//!
  bool inDet(const TVector3& v);
  int H_hit(TLorentzVector& inLV,double step);
  int NeutIn(TLorentzVector nLV,double& t,double& e);
  int C_hit(TLorentzVector& inLV,double step);
  void Reset();
  void Build();
  void SetCalibrations(double elin, 
		       double eshift,
		       double tlin,
		       double tshift,
		       double zero_off);
  void SetCalibrations(RN_VariableMap& detvar);
  double CalculateTRel(const double &tfirst);
  Int_t IsANeutron();
  Int_t IsAGamma();
  Int_t HitID();


  
  ClassDef(RN_NeutDetector,1);
};

typedef std::vector<RN_NeutDetector> RN_NeutCollection;
typedef std::vector<RN_NeutDetector>::iterator RN_NeutCollectionRef;




class RN_NeutDetectorArray:public TObject{
private:
public:
  Double32_t fT_first;
  int fDetfirst;
  int fT_mult;
  int fMult;
  std::vector<TVector3>fPos;//!
  std::vector<Double32_t>fQ_long;//[fMult]
  std::vector<Double32_t>fPSD;//[fMult]
  std::vector<Double32_t>fT;//[fMult]
  std::vector<int>fDetlist;//[fMult]



  RN_NeutDetectorArray();
  int ReconstructHits(RN_NeutCollection& in);
  int InsertHit(const double& q_long,const double& q_short,const double& q_T,const TVector3& fPos,const int& index);
  
  int Reset();
  ClassDef(RN_NeutDetectorArray,1);
};

namespace RNArray{

  int ReconstructTREL(RN_NeutCollection& in,int&t_mult,double&t_first,int& det_first);
  int PositionMap(int slot,TVector3 & pos);

}

#endif
