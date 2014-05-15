////////////////////////////////////////////
/// Sean Kuvin 2013
/// Base class for a RN_Particle, 
/// consisting of TLorentzVector
/// and calculations.
//////////////////////////////////////////


#ifndef _PARTICLE__
#define _PARTICLE__

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
#include "RN_BaseClass.hpp"

class RN_Particle:public RN_BaseClass{
private:
  double mass;
  double ex_energy;
public:
  RN_Particle():RN_BaseClass("",""){};
  RN_Particle(const std::string& pname,const double& ex = 0);
  ~RN_Particle(){};

  
  TLorentzVector LV;

  Double32_t M()const {return mass;}
  Double32_t Ex()const {return ex_energy;}
  Double32_t SetExEnergy(const double & exe){return (ex_energy = exe);};
  Double32_t KE()const {return LV.E()-LV.M();}
  void Init(const std::string & pname );
  void Reset();

  ClassDef(RN_Particle,2);
};

typedef std::vector<RN_Particle> RN_ParticleCollection;
typedef std::vector<RN_Particle>::iterator RN_ParticleCollectionRef;


#endif
