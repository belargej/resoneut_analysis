#ifndef _SIMPHYSICS__
#define _SIMPHYSICS__

//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TTree.h>
#include <TRandom3.h>

#include "RN_NeutDetectorArray.hpp"

class RN_AngularDistribution{


public:
  RN_AngularDistribution();
  RN_AngularDistribution(std::string);
  virtual ~RN_AngularDistribution();
  double ad[180];
  double GetAD(double angle);

  ClassDef(RN_AngularDistribution,1);
};



class RN_SimEvent{

public:
  RN_SimEvent();
  RN_SimEvent(double parent_mass,double rad_mass,double hi_mass);
  RN_SimEvent(double beam_energy,double beam_mass,
	     double target_mass,double rad_mass,
	     double hi_mass);
  virtual ~RN_SimEvent();
  std::vector<TLorentzVector> LVarray;  
  std::vector<double> Marray;
  double beam_energy;
  double thebeam_mass;
  double thetarget_mass;
  TVector3 boostv;
  int radiate_in_CM(const TVector3& rad_V,
		    const double& hi_ex);
  int radiate_in_CM(const TLorentzVector& in_LV,
		    const TVector3& rad_V,const double& hi_ex);

  //utility functions
  //get particle LorentzVectors
  //const LorentzVector& getLV(int i){return LVarray.at(i);}
  const TLorentzVector& getLVin(){return LVarray[0];} //beam + target or parent nucleus
  const TLorentzVector& getLVrad(){return LVarray[1];} //radiated particle
  const TLorentzVector& getLVhi(){return LVarray[2];} //daughter nucleus (heavy ion)
  //get particle ground state rest masses
  //const double& getM(int i){return Marray.at(i);}
  const double& getMin(){return Marray[0];} //beam + target or parent nucleus
  const double& getMrad(){return Marray[1];} //radiated particle
  const double& getMhi(){return Marray[2];} //daughter nucleus (heavy ion)
  
 
  ClassDef(RN_SimEvent,1);
};

class RN_PTerph:public TObject{

private:
  std::string fName;
  double fRadius;
  double fThickness;
  double fDt;
  double fT;
  double fThreshold;
  int fCounter;
  TVector3 fPos;
  TRandom3 rnd;//!
  double z_pos;//!
  double fEsum;
  
public:
  RN_PTerph(std::string,int apos);
  RN_PTerph();
  int NeutIn(TLorentzVector nlv,double& t,double& e);
  int H_hit(TLorentzVector &nlv);
  int C_hit(TLorentzVector &nlv);
  bool inDet(const TVector3& v);
 
  void Reset();
  void LoadVariables(RN_VariableMap&);
  std::string Name() const {return fName;}
  TVector3 GetPosVect()const {return fPos;}
  float GetRadius()const {return fRadius;}
  float GetThickness()const {return fThickness;}
  float GetThreshold()const{return fThreshold;}

  ClassDef(RN_PTerph,1)
};
typedef std::vector<RN_PTerph> RN_PTerphCollection;
typedef std::vector<RN_PTerph>::iterator RN_PTerphCollectionRef;



#endif
