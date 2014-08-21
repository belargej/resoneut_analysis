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
 protected:
  double fTLin; //!
  double fTShift;//!
  double fShortOffset;//!
  int fAPos;//!
  TVector3 fHitPos;//!
  TVector3 fPos;//!
  double fPheLin;//!
  double fPheShift;//!

  //simulation parameters
  double fRadius;//!
  double fThickness;//!
  double fThreshold;//!
  int fCounter;
  int fCounterCarbon;


  //NeutDetector Specific Parameterss 
  Double32_t fQLong;
  Double32_t fQShort;
  Double32_t fPSD;
  Double32_t fTQ;
  Double32_t fTRel; //relative to other detectors


 public:
  RN_NeutDetector(std::string name="",int num=4,int ap=0);

  ~RN_NeutDetector(){}

  //Simulation Parameters
  Double32_t fDt;
  Double32_t fTSim;
  Double32_t fESum;
  Double32_t fELost;


  void InsertPSDHit(const double& fql,const double& fqs,const double& t = 0.0);
 
  Double_t PSD() const ;
  Double_t QLong() const ;
  Double_t QShort() const;
  Double_t QShortOffset() const;
  Double_t QkeVee() const;  
  Double_t T() const;
  Double_t TRaw() const;
  Double_t TRel() const;
  Double_t nKE(Double_t tof) const;
  Double_t nKE_R(Double_t tof) const;
  Double_t GetRadius()const ;
  Double_t GetThickness()const;
  Double_t GetThreshold()const;
  TVector3 GetHitPos()const;
  Int_t HitCounter() const ;
  Int_t GetArrayPos() const;
  TVector3 GetPosVect() const; 
  Double_t Q_value_est(double tof,
		       double m1,
		       double m2,
		       double beam_e,
		       double& hi_KE,
		       double& Q_val);

  
  bool inDet(const TVector3& v , const TVector3& beamspot = TVector3(0,0,0));
  int H_hit(TLorentzVector& inLV,double step);
  int NeutIn(TLorentzVector nLV,double& t,double& e, const TVector3& beamspot = TVector3(0,0,0));
  int C_hit(TLorentzVector& inLV,double step);
  void Reset();
  void Build();
  void SetCalibrations(RN_VariableMap& detvar);
  double CalculateTRel(const double &tfirst);
  Int_t IsANeutron();
  Int_t IsAGamma();
  Int_t HitID();


  
  ClassDef(RN_NeutDetector,1);
};

typedef std::vector<RN_NeutDetector> RN_NeutCollection;
typedef std::vector<RN_NeutDetector>::iterator RN_NeutCollectionRef;



inline Double_t RN_NeutDetector::QLong() const{return fQLong;}
inline Double_t RN_NeutDetector::QShort() const{return fQShort;}
inline Double_t RN_NeutDetector::QShortOffset() const{return (fQShort + fShortOffset);}
inline Double_t RN_NeutDetector::TRaw() const {return fTQ;}
inline Double_t RN_NeutDetector::TRel() const {return fTRel;}
inline Double_t RN_NeutDetector::PSD() const {return fPSD;}
inline Double_t RN_NeutDetector::GetRadius()const {return fRadius;}
inline Double_t RN_NeutDetector::GetThickness()const {return fThickness;}
inline Double_t RN_NeutDetector::GetThreshold()const{return fThreshold;}
inline TVector3 RN_NeutDetector::GetHitPos()const{return fHitPos;}
inline Int_t RN_NeutDetector::HitCounter() const {return fCounter;}
inline Int_t RN_NeutDetector::GetArrayPos() const{return fAPos;}
inline TVector3 RN_NeutDetector::GetPosVect() const{return fPos;}



class RN_NeutDetectorArray:public RN_BaseClass{
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

  RN_NeutDetectorArray(const TString & name = "");
  int ReconstructHits(RN_NeutCollection& in);
  int InsertHit(const double& q_long,const double& q_short,const double& q_T,const TVector3& fPos,const int& index);
  inline Double32_t T(int i=0) const {return i<fMult ? fT[i] :0;}
  
  virtual void Reset();
  ClassDef(RN_NeutDetectorArray,1);
};

namespace RNArray{

  int ReconstructTREL(RN_NeutCollection& in,int&t_mult,double&t_first,int& det_first);
  int PositionMap(int slot,TVector3 & pos);

}


#endif
