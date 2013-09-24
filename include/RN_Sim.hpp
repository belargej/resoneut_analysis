#ifndef _RNSIM__
#define _RNSIM__
//C and C++ libraries.
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <TTree.h>
#include <TLorentzVector.h>


#include "RN_SiliconDetectors.hpp"
#include "RN_VariableMap.hpp"
#include "RN_SimPhysics.hpp"
#include "RN_Particle.hpp"
#include "RN_MassTable.hpp"

namespace sim{

class RN_Sim{

private:
  RN_AngularDistribution nDWBA;

public:

  double E_deposited;
  double n_cm;
  double n_tof;
  double fNe,fNt;

  RN_Sim(){};
  ~RN_Sim(){};

  void Init();
  void Reset();
  void SetAngularDistribution(std::string filename);
  int GenerateEvents(Long64_t evnum,std::string options);
  double QValue(const double,const double,double&,double&);
};

}

#endif
