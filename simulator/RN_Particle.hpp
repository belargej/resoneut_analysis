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

#include "core/RN_MassTable.hpp"

class RN_Particle:public TObject{
private:
  std::string fName;

public:
  RN_Particle(){};
  RN_Particle(const TLorentzVector&);
  RN_Particle(std::string);
  virtual ~RN_Particle(){};

  double mass;
  double ex_energy;
  TLorentzVector LV;
  RN_MassTable mtable;//!

  std::string const Name(){return fName;}
  
  void Reset();

  ClassDef(RN_Particle,1);
};

typedef std::vector<RN_Particle> RN_ParticleCollection;
typedef std::vector<RN_Particle>::iterator RN_ParticleCollectionRef;


#endif
