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
  std::string fName;
  double elin;//!
  double eshift;//!
  double tlin;//!
  double tshift;//! 
  std::vector<double> fronta0;//!
  std::vector<double> fronta1;//!
  std::vector<double> backa0;//!
  std::vector<double> backa1;//!
  TVector3 normv_;//!
  TVector3 shiftv_;//!
  TVector3 posv_;//!
  TVector3 rotv_;//!
  double ring_pitch_;//!
  double delta_phi_;//!
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

  }

  TVector3 GetPosVect(){return posv_+shiftv_;}
  void SetPosVect(TVector3 posv){posv_=posv;}
  void SetShiftVect(TVector3 shiftv){shiftv_=shiftv;}
  void SetRotVect(TVector3 rotv){rotv_=rotv;}

  TVector3 chVect(const double&cf,const double& cb);
  void Calcnormv();
  bool inDet(const TVector3&);
  bool Vect_to_ch(const TVector3&, double&, double&);
  void Reset();
  void SetCalibrations(double,double,double,double); 
  void ApplyCalibrations();
  void SetCalibrations(RN_VariableMap& detvar);
  std::string Name()const{return fName;} 
  ClassDef(RN_S2Detector,1);
 
};


#endif
