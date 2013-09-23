#ifndef __RNSILICON__
#define __RNSILICON__
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

#define S2INNERRAD 11.0
#define S2OUTERRAD 35.0
#define S2MATCHTHRESHOLD 0.1



class RN_S2Detector:public TObject{
private:
  std::string fName;//!
  Double32_t elin;//!
  Double32_t eshift;//!
  Double32_t tlin;//!
  Double32_t tshift;//! 
  std::vector<Double32_t> fronta0;//!
  std::vector<Double32_t> fronta1;//!
  std::vector<Double32_t> backa0;//!
  std::vector<Double32_t> backa1;//!
  TVector3 normv_;//!
  TVector3 shiftv_;//!
  TVector3 posv_;//!
  TVector3 rotv_;//!
  Double32_t ring_pitch_;//!
  Double32_t delta_phi_;//!

public:
  RN_BaseDetector front;
  RN_BaseDetector back;
  

  RN_S2Detector(){}
  ~RN_S2Detector(){}
  RN_S2Detector(std::string name,const int& fnum, const int& bnum):fName(name),
								   elin(1),
								   eshift(0),
								   tlin(1),
								   tshift(0),
								   fronta0(fnum,double(0)),
								   fronta1(fnum,double(1)),
								   backa0(bnum,double(0)),
								   backa1(bnum,double(1)),
								   normv_(0,0,0),
								   shiftv_(0,0,0),
								   posv_(0,0,0),
								   rotv_(0,0,0),
								   front("front",fnum),
								   back("back",bnum)
						  

  {
    ring_pitch_ = (S2OUTERRAD - S2INNERRAD) / static_cast<double>(front.NumOfCh());
    delta_phi_ = 360. / static_cast<double>(back.NumOfCh());
    front.SetELimits(0,3500);
    back.SetELimits(0,3500);
  }

  TVector3 GetPosVect(){return posv_+shiftv_;}
  void SetPosVect(TVector3 posv){posv_=posv;}
  void SetShiftVect(TVector3 shiftv){shiftv_=shiftv;}
  void SetRotVect(TVector3 rotv){rotv_=rotv;}
  Double_t Front_E(int i=0)const;
  Double_t Back_E(int i=0)const;
  Double_t Front_T(int i=0)const;
  Double_t Back_T(int i=0)const;
 


  TVector3 chVect(const double&cf,const double& cb) const;
  void Calcnormv();
  bool inDet(const TVector3&);
  bool Vect_to_ch(const TVector3&, double&, double&);
  void Reset();
  void SetCalibrations(double,double,double,double); 
 
  void SetCalibrations(RN_VariableMap& detvar);
  std::string Name()const{return fName;}//!
  ClassDef(RN_S2Detector,1);
 
};


typedef std::vector<RN_S2Detector> RN_S2Collection;
typedef std::vector<RN_S2Detector>::iterator RN_S2CollectionRef;





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

  RN_S2Cluster(int i=0):RN_BaseDetector("si.cluster",16),
			fChlist_b(16,-1.),
			fPos(16)
  {
  Reset();
  match_enefromback=1.0;
  match_epsilon=0.0;
  match_delta=0.1;
  match_maxene=4096;
  addback_front=0.0; 
  addback_back=0.0;
  
}
  ~RN_S2Cluster(){};
  RN_S2Cluster(std::string name,Int_t NumOfch);

  int ReconstructClusters(RN_S2Detector& in);
  int SetMatchParameters(float match_enefromback,
			 float match_epsilon,
			 float match_delta,
			 float match_maxene,
			 float addback_front, 
			 float addback_back);
  void Reset();
  void SetCalibrations(RN_VariableMap&);
    
  ClassDef(RN_S2Cluster,1);
};



typedef std::vector<RN_S2Cluster> RN_S2ClusterCollection;
typedef std::vector<RN_S2Cluster>::iterator RN_S2ClusterCollectionRef;




/////////////////////////////////////////////////////////////////
///TempList functions
////////////////////////////////////////////////////////////////



class RNTempList{
public:
  RNTempList();
  RNTempList(const unsigned short no_channels);
  ~RNTempList();
  
  int mult;
  unsigned short no_channels_;
  float *chlist;
  float *elist;
  float *tlist;
 
  void InsertHit(float e, float t, float ch);
};





#endif
