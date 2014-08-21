/*************************************************************************
The methods for sorting the silicon detector hits are
contained here. 
We have three main classes:
*RN_S2Detector
*RN_S2Cluster
*RN_SiArray

The S2Detector stores the raw data, calibrations, and methods to apply the calibrations.
The S2Cluster stores the reconstructed data which requires a matching front hit and a matching back hit from the S2Detectors.
The RN_SiArray organizes the telescope information from events for instance:
RN_SiArray::E_AB() returns the sum of the first 2 reconstructed cluster results.  Silicon DE-E would then be a plot of E_A():E_AB().

We also define RN_S2Collection as a std::vector of S2Detectors
and S2ClusterCollection as a vector of S2Clusters.
 
The S2Detectors are divided into 2 RN_BaseDetectors for
the rings(front) and segments(back). When Sorting hits into these detectors
we must call the appropriate one. For Instance:

RN_S2Collection si_;//create the vector container
si_.push_back(RN_S2Detector("si_a",16,16); //add the detector
si_[0].front.InsertHit(ADC2[18],0,2); // e,t,ch
si_[0].back.InsertHit(ADC2[2],TDC2[2],2); //e,t,ch
 
We then pass the detector to the cluster using S2Cluster::ReconstructClusters
RN_S2ClusterCollection si_cluster_;
si_cluster_.push_back(RN_S2Cluster("si_cluster_a",16));
si_cluster_[0].ReconstructClusters(si_[0]);

Finally, we pass the entire cluster collection to the SiArray::ReconstructHits

RN_SiArray si_array("si_array",numofsidetectors); 
si_array.ReconstructHits(si_cluster_);
 

--Setting the calibration parameters follow the method detailed
in RN_BaseDetector, add the <string,double> pair to the 
RN_VariableMap as it is looked for by the strings in this->SetCalibrations().

Author: Sean A. Kuvin 2013
 
**************************************************************************/
#ifndef __RNSILICON__H
#define __RNSILICON__H

//C and C++ libraries.
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

//ROOT libraties
#include <TString.h>
#include <TMath.h>
#include <TVector3.h>

#include "RN_VariableMap.hpp"
#include "RN_BaseDetector.hpp"


#define S2INNERRAD 11.0
#define S2OUTERRAD 35.0
#define S2MATCHTHRESHOLD 0.1
#define S1INNERRAD 24.0
#define S1OUTERRAD 48.0
#define S1MATCHTHRESHOLD 0.1



class RN_S2Detector:public RN_BaseClass{
protected:
  Double32_t fELin;//!
  Double32_t fEShift;//!
  Double32_t fTLin;//!
  Double32_t fTShift;//! 
  TVector3 normv_;//!
  TVector3 shiftv_;//!
  TVector3 posv_;//!
  TVector3 rotv_;//!
  Double32_t ring_pitch_;//!
  Double32_t delta_phi_;//!
  Double32_t innerrad;//!
  Double32_t outerrad;//!
  Int_t _s1switch;//!
public:
  RN_BaseDetector front;
  RN_BaseDetector back;
  
  ~RN_S2Detector(){}
  RN_S2Detector(std::string name= "S2",const int& fnum = 16, const int& bnum = 16);
  
  TVector3 GetPosVect()const{return posv_ + shiftv_ ;}
  void SetPosVect(TVector3 posv){posv_=posv;}
  void SetShiftVect(TVector3 shiftv){shiftv_=shiftv;}
  void SetRotVect(TVector3 rotv){rotv_=rotv;}
  Double_t Front_E(unsigned int i=0)const;
  Double_t Back_E(unsigned int i=0)const;
  Double_t Front_T(unsigned int i=0)const;
  Double_t Back_T(unsigned int i=0)const;
  Double_t InnerTheta()const;
  Double_t OuterTheta()const;
  //int Quadrant(unsigned int i=0)const;
  //int Side(unsigned int i=0)const;
  //Double_t Ring_Ch(unsigned int i=0)const;


  virtual TVector3 chVect(const double&cf,const double& cb) const;
  void Calcnormv();
  bool inDet(const TVector3& incident,const TVector3& beam = TVector3(0,0,0));
  bool Vect_to_ch(const TVector3&, double&, double&, const TVector3& = TVector3(0,0,0));
  void Reset();
  void SetCalibrations(double,double,double,double); 
  Int_t IsS1()const;
  
  void SetCalibrations(RN_VariableMap& detvar);
  std::string Name()const{return GetName();}//!
  ClassDef(RN_S2Detector,3);
 
};

inline Int_t RN_S2Detector::IsS1() const {return _s1switch;}


typedef std::vector<RN_S2Detector> RN_S2Collection;
typedef std::vector<RN_S2Detector>::iterator RN_S2CollectionRef;


class RN_S1Detector:public RN_S2Detector{
private:
public:
  
  RN_S1Detector(std::string name = "S1",const int& fnum = 16, const int& bnum =16):RN_S2Detector(name,fnum,bnum)							   
  {
    outerrad=S1OUTERRAD;
    innerrad=S1INNERRAD;
    _s1switch=1;//needed to correct for mistake in cable plugin
    ring_pitch_ = (S1OUTERRAD - S1INNERRAD) / static_cast<double>(front.NumOfCh());
    delta_phi_ = 360. / static_cast<double>(back.NumOfCh());
  }
  
  ClassDef(RN_S1Detector,1);
};





class RN_S2Cluster:public RN_BaseDetector{
private:
  float efrontmatch;//!
  float ebackmatch;//!
  unsigned int frontmatchstat;//!
  unsigned int backmatchstat;//!
  float match_enefromback;//!
  float match_epsilon;//!
  float match_delta;//!
  float match_maxene;//!
  float addback_front; //!
  float addback_back;//!
  

public:
  std::vector<Double32_t>fChlist_b;
  std::vector<TVector3> fPos;

  ~RN_S2Cluster(){};
  RN_S2Cluster(std::string name = "S2Cluster",Int_t NumOfch = 16);

  virtual int ReconstructClusters(RN_S2Detector& in);
  int SetMatchParameters(float match_enefromback,
			 float match_epsilon,
			 float match_delta,
			 float match_maxene,
			 float addback_front, 
			 float addback_back);
  void Reset();
  void SetCalibrations(RN_VariableMap&);
  Double_t Phi(int i=0)const {return fPos[i].Phi();};
  Double_t Theta(int i=0)const {return fPos[i].Theta();};
  
  ClassDef(RN_S2Cluster,2);
};



typedef std::vector<RN_S2Cluster> RN_S2ClusterCollection;
typedef std::vector<RN_S2Cluster>::iterator RN_S2ClusterCollectionRef;



class RN_S1Cluster:public RN_S2Cluster{
private:
  
public:
  
  RN_S1Cluster(std::string name = "S1Cluster",Int_t NumOfch =16):RN_S2Cluster(name,NumOfch){}
  
  virtual int ReconstructClusters(RN_S1Detector& in){return false;};
  ClassDef(RN_S1Cluster,2);
};



class RN_SiArray:public RN_BaseClass{
private:
  Int_t fNumOfSi;
  Double32_t fP[4]; //! pol2 fit of eloss
  std::vector<Double32_t> fE_;
  std::vector<TVector3> fPos_;
  std::vector<Double32_t> fT_;
 
public:

  RN_SiArray(const std::string& name = "SiArray", int num = 2);

  void ReconstructHits(RN_S2ClusterCollection& si_c_);
  void Reset();

  Double32_t E_A()const {return fNumOfSi > 0 ? fE_[0] : 0;}
  Double32_t E_B()const {return fNumOfSi > 1 ? fE_[1] : 0;}
  Double32_t E_AB()const {return fNumOfSi > 1 ? fE_[1] + fE_[0] : 0;}
  Double32_t T_A()const {return fNumOfSi > 0 ? fT_[0] : 0;}
  Double32_t T_B()const {return fNumOfSi > 1 ? fT_[1] : 0;}
  Double32_t Theta_A()const {return fNumOfSi > 0 ? fPos_[0].Theta() : 0;}
  Double32_t Theta_B()const {return fNumOfSi > 1 ? fPos_[1].Theta() : 0;}
  Double32_t Phi_A()const {return fNumOfSi > 0 ? fPos_[0].Phi() : 0;}
  Double32_t Phi_B()const {return fNumOfSi > 1 ? fPos_[1].Phi() : 0;}
  Int_t NumOfSi()const {return fNumOfSi;}
  void SetCalibrations(RN_VariableMap & detvar);


  Double32_t ERecoAB() const{return fNumOfSi > 1 ? (fE_[1] + ERecoA()) : 0;}
  Double32_t ERecoA() const{return fNumOfSi > 1 ? ( (fP[3] * TMath::Power(fE_[1],3) ) + (fP[2] * TMath::Power(fE_[1],2)) + (fP[1] * fE_[1]) + fP[0]) : 0;}

  ClassDef(RN_SiArray,1);
};





/////////////////////////////////////////////////////////////////
///TempList functions
////////////////////////////////////////////////////////////////



class RNTempList{
public:
  RNTempList();
  RNTempList(const unsigned short no_channels);
  ~RNTempList();
  
  unsigned int mult;
  unsigned short no_channels_;
  float *chlist;
  float *elist;
  float *tlist;
 
  void InsertHit(float e, float t, float ch);
};




#endif
